INCLUDE(${PROJECT_SOURCE_DIR}/cmake/ansi.cmake)
# 1. 定义一个 components 列表变量，其值为 name/version 的列表
SET(components
    BasicTypes/V1.1.0
    BeeBase/V1.10.0
    BeeProto/V1.1.1
    Beehlog/V2.4.19
    Beesafefunc/V1.0.0
    boost/V1.69.0
    DBLayer/V2.0.0
    hpr/V1.1.15
    libxml/V2.9.12
)

FOREACH(component ${components})
  SET(full_path $ENV{HPT_DEPS}/${component})
  MESSAGE(STATUS "${BLU}添加依赖 ${CYNB}[${component}]\t${BLUB} -> ${WHT}${full_path}${RST}")
  INCLUDE_DIRECTORIES(${full_path}/include)
  LINK_DIRECTORIES(${TARGET_NAME} PUBLIC ${full_path}/bin)
  FILE(GLOB_RECURSE dll_files ${full_path}/bin/*.dll)
  FOREACH(dll_file ${dll_files})
    # MESSAGE(STATUS ${BLUB}复制${dll_file}${RST})
    GET_FILENAME_COMPONENT(dll_name ${dll_file} NAME)
    FILE(COPY ${dll_file} DESTINATION ${TARGET_DIR})
  ENDFOREACH()
ENDFOREACH()
