function(getProjectVersion lib config_file_path)

    ### Get major.minor.patch semantic version
    file(READ ${config_file_path} config_file)

    string(REGEX MATCH "define[ \t]+HANDY_MAJOR_VERSION[ \t]+([0-9]+)" lib_name_major_version_match ${config_file})
    set(${lib}_name_major_version "${CMAKE_MATCH_1}")

    string(REGEX MATCH "define[ \t]+HANDY_MINOR_VERSION[ \t]+([0-9]+)" lib_name_minor_version_match ${config_file})
    set(${lib}_name_minor_version "${CMAKE_MATCH_1}")

    string(REGEX MATCH "define[ \t]+HANDY_PATCH_VERSION[ \t]+([0-9]+)" lib_name_patch_version_match ${config_file})
    set(${lib}_name_patch_version "${CMAKE_MATCH_1}")

    set(${lib}_full_version ${${lib}_name_major_version}.${${lib}_name_minor_version}.${${lib}_name_patch_version} PARENT_SCOPE)
    ###

endfunction()