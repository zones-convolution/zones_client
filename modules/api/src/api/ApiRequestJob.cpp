#include "ApiRequestJob.h"

ApiRequestJob::ApiRequestJob (ApiRequest api_request, Callbacks callbacks)
    : juce::ThreadPoolJob ("ApiDownloadJob")
    , api_request_ (std::move (api_request))
    , callbacks_ (std::move (callbacks))
{
}

void ApiRequestJob::NotifyStart ()
{
    if (callbacks_.start == nullptr)
        return;
    callbacks_.start ();
}

void ApiRequestJob::NotifyProgress ()
{
    if (callbacks_.progress == nullptr)
        return;

    auto progress = 0.0f;
    auto streamLength = input_stream_->getTotalLength ();
    if (streamLength > 0)
        progress =
            juce::jlimit (0.0f, 1.0f, float (memory_block_.getSize ()) / float (streamLength));

    if (std::fabsf (progress - last_progress_) > 0.15f)
    {
        last_progress_ = progress;
        callbacks_.progress (progress);
    }
}

void ApiRequestJob::NotifySuccess ()
{
    if (callbacks_.success == nullptr)
        return;
    callbacks_.success ({.data = memory_block_, .status = status_code_});
}

void ApiRequestJob::NotifyFail ()
{
    if (callbacks_.fail == nullptr)
        return;
    callbacks_.fail ({.data = memory_block_, .status = status_code_});
}

void ApiRequestJob::CreateInputStream ()
{
    auto urlRequest = api_request_.GetUrlRequest ();

    bool hasFields = (api_request_.fields.getProperties ().size () > 0);
    if (hasFields)
    {
        juce::MemoryOutputStream fieldOutputStream;
        api_request_.fields.writeAsJSON (fieldOutputStream, 0, false, 20);
        urlRequest = urlRequest.withPOSTData (fieldOutputStream.toString ());
    }

    input_stream_ = urlRequest.createInputStream (
        juce::URL::InputStreamOptions (juce::URL::ParameterHandling::inAddress)
            .withExtraHeaders (ApiRequest::StringPairArrayToHeaderString (api_request_.headers))
            .withHttpRequestCmd (ApiRequest::GetHttpVerbLiteral (api_request_.http_verb))
            .withStatusCode (&status_code_)
            .withResponseHeaders (&response_headers_));
}

static bool isSuccessHttpStatusCode (int httpStatusCode)
{
    return httpStatusCode >= 200 && httpStatusCode < 300;
}

void ApiRequestJob::ProcessStream ()
{
    while (! shouldExit () && ! input_stream_->isExhausted ())
    {
        input_stream_->readIntoMemoryBlock (memory_block_, kMaxNumBytesToRead);
        NotifyProgress ();
    }

    if (! shouldExit () && isSuccessHttpStatusCode (status_code_))
        NotifySuccess ();
    else
        NotifyFail ();
}

juce::ThreadPoolJob::JobStatus ApiRequestJob::runJob ()
{
    NotifyStart ();
    CreateInputStream ();
    ProcessStream ();

    return jobHasFinished;
}
