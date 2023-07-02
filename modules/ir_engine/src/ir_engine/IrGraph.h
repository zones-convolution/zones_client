#pragma once
#include <array>
#include <juce_dsp/juce_dsp.h>
#include <string>

struct IrGraphState
{
    float param_1;
    int param_2;
    std::string param_3;
};

class IrGraphCachePolicy
{
public:
    [[nodiscard]] virtual std::size_t GetHashForState (const IrGraphState & state) const = 0;
    virtual ~IrGraphCachePolicy () = default;
};

class IrGraphProcessor
{
public:
    virtual void Process (juce::dsp::ProcessContextNonReplacing<float> & process_context);
    virtual ~IrGraphProcessor () = default;
};

class TestProcessor : public IrGraphProcessor
{
public:
    void Process (juce::dsp::ProcessContextNonReplacing<float> & process_context) override;
};

class TestCachePolicy : public IrGraphCachePolicy
{
public:
    std::size_t GetHashForState (const IrGraphState & state) const override;
};

using ProcessorWithCachePolicy = std::pair<IrGraphCachePolicy &, IrGraphProcessor &>;

class IrGraph
{
public:
private:
    [[nodiscard]] std::vector<std::size_t>
    GetHashesForState (const IrGraphState & ir_graph_state) const;

    [[nodiscard]] std::size_t CombineProcessorHashes (size_t previous_processor_hash,
                                                      size_t next_processor_hash) const;

    [[nodiscard]] std::size_t GetHashForCachePolicy (const IrGraphCachePolicy & cache_policy,
                                                     size_t previous_processor_hash,
                                                     const IrGraphState & ir_graph_state) const;

    TestCachePolicy test_cache_policy {};
    TestProcessor test_processor {};

    std::array<ProcessorWithCachePolicy, 1> processors_ {
        ProcessorWithCachePolicy {test_cache_policy, test_processor}};
};