LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/../SDL2_image $(LOCAL_PATH)/../SDL2_mixer


# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	PGL/PGL_funciones.cpp \
	PGL/PGL_compresor.cpp \
	PGL/PGL_color.cpp \
	PGL/PGL_rectangulo.cpp \
	PGL/PGL_cadena.cpp \
	PGL/PGL_archivo.cpp \
	PGL/PGL_tiempo.cpp \
	PGL/PGL_sistema.cpp \
	PGL/PGL_mapaxml.cpp \
	PGL/PGL_ventana.cpp \
	PGL/PGL_imagen.cpp \
	PGL/PGL_musica.cpp \
	PGL/PGL_sonido.cpp \
	basePGL.cpp \
	baseTMP.cpp \
	control.cpp \
	sonidos.cpp \
	musicas.cpp \
	baldosas.cpp \
	mapa.cpp \
	dibuja.cpp \
	monje.cpp \
	disparos.cpp \
	juego.cpp \
	enemigos.cpp \
	procesos.cpp \
	main.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lz

#LOCAL_CFLAGS := 
LOCAL_CPPFLAGS := -DPGL_ANDROID

include $(BUILD_SHARED_LIBRARY)
