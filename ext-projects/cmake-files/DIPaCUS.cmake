ExternalProject_Add(dipacus
        GIT_REPOSITORY https://github.com/danoan/DIPaCUS.git
        GIT_TAG v0.1
        SOURCE_DIR ${EXTPROJECTS_SOURCE_DIR}/DIPaCUS
        CMAKE_ARGS
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX=${EXTPROJECTS_BUILD_DIR}
        -DCREATE_SHARED_LIBRARIES=ON)