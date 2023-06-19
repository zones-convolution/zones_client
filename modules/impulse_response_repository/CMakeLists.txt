zones_add_module(
        MODULE ir_repository
        SOURCES src/impulse_response_repository/io/ImpulseResponseReader.h
        src/impulse_response_repository/io/ImpulseResponseReader.cpp
        src/impulse_response_repository/io/ImpulseResponseWriter.h
        src/impulse_response_repository/io/ImpulseResponseWriter.cpp
        src/impulse_response_repository/io/ScopedImpulseResponseIo.h
        src/impulse_response_repository/io/ScopedImpulseResponseIo.cpp
        DEPS juce::juce_audio_basics
)

zones_add_test(
        MODULE ir_repository
        SOURCES test/TestTestModuleClass.cpp
        DEPS zones_test_module
)