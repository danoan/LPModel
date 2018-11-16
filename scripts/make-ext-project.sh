ROOT_DIR=$PWD
INSTALL_PATH=$ROOT_DIR/ext
EXT_PROJECT_DIR=$ROOT_DIR/ext-project

mkdir $EXT_PROJECT_DIR -p

#--------------DIPaCUS----------------

DIPACUS_DIR=$EXT_PROJECT_DIR/DIPaCUS

DIPACUS_BUILD_DIR=$DIPACUS_DIR/build
mkdir $DIPACUS_BUILD_DIR
cd $DIPACUS_BUILD_DIR
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH  -DCMAKE_PREFIX_PATH=/home-local/dantu1/Documents/local_libs/lib
make
make install


#--------------GEOC----------------

GEOC_DIR=$EXT_PROJECT_DIR/geoc

GEOC_BUILD_DIR=$GEOC_DIR/build
mkdir $GEOC_BUILD_DIR
cd $GEOC_BUILD_DIR
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH -DCMAKE_PREFIX_PATH=/home-local/dantu1/Documents/local_libs/lib
make
make install



#--------------SCaBOliC----------------

SCABOLIC_DIR=$EXT_PROJECT_DIR/SCaBOliC

SCABOLIC_BUILD_DIR=$SCABOLIC_DIR/build
mkdir $SCABOLIC_BUILD_DIR
cd $SCABOLIC_BUILD_DIR
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH -DCMAKE_PREFIX_PATH=/home-local/dantu1/Documents/local_libs/lib  \
-DDIPACUS_INCLUDE_DIRS=$INSTALL_PATH/include \
-DDIPACUS_LIBS_DIR=$INSTALL_PATH/lib \
-DGEOC_INCLUDE_DIRS=$INSTALL_PATH/include 
make
make install



#--------------BinOCS----------------

BINOCS_DIR=$EXT_PROJECT_DIR/BinOCS

BINOCS_BUILD_DIR=$BINOCS_DIR/build
mkdir $BINOCS_BUILD_DIR
cd $BINOCS_BUILD_DIR
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH -DCMAKE_PREFIX_PATH=/home-local/dantu1/Documents/local_libs/lib \
-DDIPACUS_INCLUDE_DIRS=$INSTALL_PATH/include \
-DDIPACUS_LIBS_DIR=$INSTALL_PATH/lib \
-DSCABOLIC_INCLUDE_DIRS=$INSTALL_PATH/include \
-DSCABOLIC_LIBS_DIR=$INSTALL_PATH/lib 
make
make install
