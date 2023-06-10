include(FetchContent)

macro(update_from_git name repo branch)
    FetchContent_Declare(
            ${name}
            GIT_REPOSITORY ${repo}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_TAG "${branch}")

    FetchContent_GetProperties(${name})

    if (NOT ${name}_POPULATED)
        message("Updating ${name} from git...")
        FetchContent_Populate(${name})
        message("${name} update finished")
    endif ()
endmacro()
