target_sources(${LEVEL_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Database.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DBElement.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DBException.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DBView.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IDatabase.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IIndexer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Indexer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IQuery.hpp
)

add_subdirectory(indexer)
add_subdirectory(queries)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})