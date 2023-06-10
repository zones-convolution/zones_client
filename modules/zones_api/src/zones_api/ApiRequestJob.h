#pragma once
#include "ApiRequest.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
namespace zones
{
class ApiRequestJob : public juce::ThreadPoolJob
{
public:
    struct Result
    {
        juce::MemoryBlock data;
        int status;
    };

    struct Callbacks
    {
        using Success = std::function<void (Result)>;
        using Fail = std::function<void (Result)>;
        using Progress = std::function<void (float)>;
        using Start = std::function<void ()>;

        Success success = nullptr;
        Fail fail = nullptr;
        Progress progress = nullptr;
        Start start = nullptr;
    };

    ApiRequestJob (ApiRequest apiRequest, Callbacks callbacks);
    ~ApiRequestJob () override = default;
    JobStatus runJob () override;

private:
    static constexpr int m_maxNumBytesToRead = 1024 * 8;

    ApiRequest m_apiRequest;
    const Callbacks m_callbacks;
    int m_statusCode;
    juce::StringPairArray m_responseHeaders;
    juce::MemoryBlock m_memoryBlock;
    float m_lastProgress = 0.f;

    std::unique_ptr<juce::InputStream> m_inputStream;

    void notifyStart ();
    void notifyProgress ();
    void notifySuccess ();
    void notifyFail ();

    void createInputStream ();
    void processStream ();
};
}