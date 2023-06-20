add_subdirectory(JUCE)

update_from_git(immer "https://github.com/arximboldi/immer"
        "0b3aaf699b9d6f2e89f8e2b6d1221c307e02bda3")
add_library(immer INTERFACE)
target_include_directories(immer INTERFACE "${immer_SOURCE_DIR}")

update_from_git(zug "https://github.com/arximboldi/zug"
        "d7e814b45fceceee3cb1442997d8b46cee4764ec")
add_library(zug INTERFACE)
target_include_directories(zug INTERFACE "${zug_SOURCE_DIR}")

update_from_git(lager "https://github.com/arximboldi/lager"
        "8fd338d6d67dd2efc4f38c0634c2f849a7db8774")
add_library(lager INTERFACE)
target_include_directories(lager INTERFACE "${lager_SOURCE_DIR}")
target_link_libraries(lager INTERFACE zug immer Boost::headers)