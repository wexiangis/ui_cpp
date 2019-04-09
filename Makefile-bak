
CROSS_COMPILE := 
CROSS :=gcc_4_8_4

#默认使用 g++
GCC=$(CROSS_COMPILE)gcc
GPP=$(CROSS_COMPILE)g++

#当前工程的根目录路径
ROOT_DIR=$(shell pwd)

#.o文件存放路径, 方便清理
OBJ_DIR=$(ROOT_DIR)/obj

appname=app

#不同编译器用不同的库, 主要是 iconv 库
INC = -I$(ROOT_DIR)/inc
LIB = -lpthread -liconv -lm -ljpeg -lfreetype -lwpa_client

INC += -I$(ROOT_DIR)/lib/iconv/iconv_for_$(CROSS)/include
LIB += -L$(ROOT_DIR)/lib/iconv/iconv_for_$(CROSS)/lib
INC += -I$(ROOT_DIR)/lib/jpeg/jpeg_for_$(CROSS)/include
LIB += -L$(ROOT_DIR)/lib/jpeg/jpeg_for_$(CROSS)/lib
INC += -I$(ROOT_DIR)/lib/freeType2/freeType2_for_$(CROSS)/include
INC += -I$(ROOT_DIR)/lib/freeType2/freeType2_for_$(CROSS)/include/freetype
LIB += -L$(ROOT_DIR)/lib/freeType2/freeType2_for_$(CROSS)/lib
INC += -I$(ROOT_DIR)/lib/wpa_client/wpa_client_for_$(CROSS)/include
LIB += -L$(ROOT_DIR)/lib/wpa_client/wpa_client_for_$(CROSS)/lib

#把所有 src/*.cpp 转换为 obj/*.o 并添加到obj
obj =${patsubst %.cpp,$(OBJ_DIR)/%.cpp.o,${notdir ${wildcard $(ROOT_DIR)/src/*.cpp}}}
#把所有 src/*.c 转换为 obj/*.o 并添加到obj
obj += ${patsubst %.c,$(OBJ_DIR)/%.o,${notdir ${wildcard $(ROOT_DIR)/src/*.c}}}

#编译所用引用到的.cpp为.o文件
%.cpp.o:../src/%.cpp
	$(GPP) -O3 -Wall -c $< $(INC) $(LIB) -o $@
%.o:../src/%.c
	$(GCC) -O3 -Wall -c $< $(INC) $(LIB) -o $@

$(appname):$(obj)
	@$(GPP) -O3 -Wall -o $(ROOT_DIR)/$@ $(obj) $(INC) $(LIB)

clean:
	@rm -rf $(ROOT_DIR)/$(appname) $(OBJ_DIR)/*.o 
