#pragma once
#include "ApiRequest.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

class ApiRequestJob : public juce::ThreadPoolJob
{
public:
    struct Result
    {
        juce::MemoryBlock data;
        int status;
    };

    using Callbacks = ApiRequestCallbacks<Result, Result>;

    ApiRequestJob (ApiRequest api_request, Callbacks callbacks);
    ~ApiRequestJob () override = default;
    JobStatus runJob () override;

private:
    static constexpr int kMaxNumBytesToRead = 1024 * 8;

    ApiRequest api_request_;
    const Callbacks callbacks_;
    int status_code_;
    juce::StringPairArray response_headers_;
    juce::MemoryBlock memory_block_;
    float last_progress_ = 0.f;

    std::unique_ptr<juce::InputStream> input_stream_;

    void NotifyStart ();
    void NotifyProgress ();
    void NotifySuccess ();
    void NotifyFail ();

    void CreateInputStream ();
    void ProcessStream ();
};