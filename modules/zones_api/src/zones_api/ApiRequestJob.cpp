#include "ApiRequestJob.h"

namespace zones
{
ApiRequestJob::ApiRequestJob (ApiRequest apiRequest, Callbacks callbacks)
    : juce::ThreadPoolJob ("ApiDownloadJob")
    , m_apiRequest (std::move (apiRequest))
    , m_callbacks (std::move (callbacks))
{
}

void ApiRequestJob::notifyStart ()
{
    if (m_callbacks.start == nullptr)
        return;

    juce::MessageManager::callAsync (
        [startCallback = m_callbacks.start] ()
        {
            if (startCallback != nullptr)
                startCallback ();
        });
}

void ApiRequestJob::notifyProgress ()
{
    if (m_callbacks.progress == nullptr)
        return;

    auto progress = 0.0f;
    auto streamLength = m_inputStream->getTotalLength ();
    if (streamLength > 0)
        progress =
            juce::jlimit (0.0f, 1.0f, float (m_memoryBlock.getSize ()) / float (streamLength));

    if (std::fabsf (progress - m_lastProgress) > 0.15f)
    {
        m_lastProgress = progress;
        juce::MessageManager::callAsync (
            [progressCallback = m_callbacks.progress, progress] ()
            {
                if (progressCallback != nullptr)
                    progressCallback (progress);
            });
    }
}

void ApiRequestJob::notifySuccess ()
{
    if (m_callbacks.success == nullptr)
        return;

    juce::MessageManager::callAsync (
        [successCallback = m_callbacks.success, data = m_memoryBlock, status = m_statusCode] ()
        {
            if (successCallback != nullptr)
                successCallback ({.data = data, .status = status});
        });
}

void ApiRequestJob::notifyFail ()
{
    if (m_callbacks.fail == nullptr)
        return;

    juce::MessageManager::callAsync (
        [failCallback = m_callbacks.fail, data = m_memoryBlock, status = m_statusCode] ()
        {
            if (failCallback != nullptr)
                failCallback ({.data = data, .status = status});
        });
}

void ApiRequestJob::createInputStream ()
{
    auto urlRequest = m_apiRequest.getUrlRequest ();

    bool hasFields = (m_apiRequest.fields.getProperties ().size () > 0);
    if (hasFields)
    {
        juce::MemoryOutputStream fieldOutputStream;
        m_apiRequest.fields.writeAsJSON (fieldOutputStream, 0, false, 20);
        urlRequest = urlRequest.withPOSTData (fieldOutputStream.toString ());
    }

    m_inputStream = urlRequest.createInputStream (
        juce::URL::InputStreamOptions (juce::URL::ParameterHandling::inAddress)
            .withExtraHeaders (ApiRequest::stringPairArrayToHeaderString (m_apiRequest.headers))
            .withHttpRequestCmd (ApiRequest::getHttpVerbLiteral (m_apiRequest.httpVerb))
            .withStatusCode (&m_statusCode)
            .withResponseHeaders (&m_responseHeaders));
}

static bool isSuccessHttpStatusCode (int httpStatusCode)
{
    return httpStatusCode >= 200 && httpStatusCode < 300;
}

void ApiRequestJob::processStream ()
{
    while (! shouldExit () && ! m_inputStream->isExhausted ())
    {
        m_inputStream->readIntoMemoryBlock (m_memoryBlock, m_maxNumBytesToRead);
        notifyProgress ();
    }

    if (! shouldExit () && isSuccessHttpStatusCode (m_statusCode))
        notifySuccess ();
    else
        notifyFail ();
}

juce::ThreadPoolJob::JobStatus ApiRequestJob::runJob ()
{
    notifyStart ();
    createInputStream ();
    processStream ();

    return jobHasFinished;
}
}