cmake_minimum_required (VERSION 2.6)

project (list-test)
project (rbtree-test)

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set (TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set (SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set (INC_DIR ${CMAKE_SOURCE_DIR}/include)

set (LIST_TEST_SRCS ${TEST_DIR}/list_test.c ${SRC_DIR}/list.c)
set (RBT_TEST_SRCS ${TEST_DIR}/rbtree_test.c ${SRC_DIR}/rb_tree.c ${SRC_DIR}/rbt_helpers.c)
set (DEQUE_TEST_SRCS ${TEST_DIR}/deque_test.c ${SRC_DIR}/deque.c ${SRC_DIR}/list.c ${SRC_DIR}/vector.c)
set (VECTOR_TEST_SRCS ${TEST_DIR}/vectortest.c ${SRC_DIR}/vector.c)
set (HASHSET_TEST_SRCS ${TEST_DIR}/hashsettest.c ${SRC_DIR}/hashset.c ${SRC_DIR}/vector.c)
set (THESAURUS_LOOKUP_SRCS ${SRC_DIR}/thesaurus-lookup.c ${SRC_DIR}/vector.c ${SRC_DIR}/hashset.c ${SRC_DIR}/streamtokenizer.c)


add_executable(list-test ${LIST_TEST_SRCS})
target_include_directories(list-test 
    PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_link_libraries(list-test cmocka)

add_executable(rbt-test ${RBT_TEST_SRCS})
target_include_directories(rbt-test 
    PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_link_libraries(rbt-test cmocka)

add_executable(deque-test ${DEQUE_TEST_SRCS})
target_include_directories(deque-test 
    PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_link_libraries(deque-test cmocka)

add_executable(vector-test ${VECTOR_TEST_SRCS})
target_include_directories(vector-test 
    PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

add_executable(hashset-test ${HASHSET_TEST_SRCS})
target_include_directories(hashset-test 
    PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

add_executable(thesaurus-lookup ${THESAURUS_LOOKUP_SRCS})
target_include_directories(thesaurus-lookup
    PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)


