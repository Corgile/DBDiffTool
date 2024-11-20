SET(_packaging_dir ${PROJECT_BINARY_DIR}/target)
SET(_packaging_filename ${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}.zip)
ADD_CUSTOM_TARGET(
    PackAndOpen
    COMMAND ${CMAKE_COMMAND} -E make_directory "${_packaging_dir}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${TARGET_DIR}" "${_packaging_dir}"
    COMMAND ${CMAKE_COMMAND} -E tar cf ${_packaging_filename} --format=zip -- "${_packaging_dir}"
    COMMAND ${CMAKE_COMMAND} -E echo "\t- create archive:${_packaging_filename}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${_packaging_dir}"
    COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ${CMAKE_COMMAND} -E echo "\t- opening location..."
    COMMAND start "" "${PROJECT_SOURCE_DIR}/"
    COMMENT "打包成果物并打开所在路径"
)
SET_TARGET_PROPERTIES(PackAndOpen PROPERTIES FOLDER "Custom")