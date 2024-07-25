#include "app/app.h"
#include "app/window.h"
#include "cimgui.h"
#include "cimgui_impl.h"
#include "assets/assets.h"
#include "log/log.h"

int AppInit(void) {
	// Initialize Window & ImGui
	if (WindowCreate("csprite", 320, 240, 1)) {
		return 1;
	}

	// Initialize Font
	int fontDataSize = 0;
	ImVector_ImWchar FontRanges;
	const ImGuiIO* io = igGetIO();
	ImVector_ImWchar_Init(&FontRanges);
	ImFontGlyphRangesBuilder *FontBuilder = ImFontGlyphRangesBuilder_ImFontGlyphRangesBuilder();
	ImFontGlyphRangesBuilder_AddRanges(FontBuilder, ImFontAtlas_GetGlyphRangesDefault(io->Fonts));
	ImFontGlyphRangesBuilder_BuildRanges(FontBuilder, &FontRanges);
	ImFontAtlas_AddFontFromMemoryCompressedTTF(
	    io->Fonts, assets_get("data/fonts/Inter.ttf", &fontDataSize),
		fontDataSize, 18, NULL, FontRanges.Data
	);
	ImFontAtlas_Build(io->Fonts);
	if (!ImFontAtlas_IsBuilt(io->Fonts)) {
		log_error("io.Fonts->Build() - failed to build the font atlas");
		return 1;
	}
	ImFontGlyphRangesBuilder_destroy(FontBuilder);
	ImVector_ImWchar_UnInit(&FontRanges);

	// Initialize Styles
	ImGuiStyle *style = igGetStyle();
	#define ImColor(r, g, b) (ImVec4){ r/255.0f, g/255.0f, b/255.0f, 1 }
	style->Colors[ImGuiCol_Button] = ImColor(44, 44, 44);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(55, 55, 55);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(66, 66, 66);

	style->Colors[ImGuiCol_TitleBg] = ImColor(0xFF, 0xAA, 0xFF);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(0xAA, 0x55, 0xAA);

	style->Colors[ImGuiCol_CheckMark] = ImColor(0xAA, 0xAA, 0xAA);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(0xAA, 0xAA, 0xAA);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(0xFF, 0xFF, 0xFF);

	style->Colors[ImGuiCol_FrameBg] = ImColor(44, 44, 44);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(55, 55, 55);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(66, 66, 66);

	style->Colors[ImGuiCol_Header] = ImColor(55, 55, 55);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(66, 66, 66);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(77, 77, 77);

	WindowSetBG(
		style->Colors[ImGuiCol_WindowBg].x * 255.0f,
		style->Colors[ImGuiCol_WindowBg].y * 255.0f,
		style->Colors[ImGuiCol_WindowBg].z * 255.0f
	);
	WindowSetMaxFPS(60);

	return 0;
}

int AppMainLoop(void) {
	while (!WindowShouldClose()) {
		WindowNewFrame();

		igBeginMainMenuBar();
			if (igBeginMenu("File", true)) {
				igEndMenu();
			}
			if (igBeginMenu("Edit", true)) {
				igEndMenu();
			}
			if (igBeginMenu("Help", true)) {
				if (igMenuItem_Bool("About", NULL, false, true)) {
					AppOpenURL("https://csprite.github.io");
				}
				igEndMenu();
			}
		igEndMainMenuBar();

		WindowEndFrame();
	}
	return 0;
}

void AppDestroy(void) {
	WindowDestroy();
}

#if defined(TARGET_WINDOWS)
	#include <windows.h>
	#include <shellapi.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AppOpenURL(const char* url) {
#if defined(TARGET_WINDOWS)
	ShellExecute(0, 0, url, 0, 0, SW_SHOW);
#elif defined(TARGET_APPLE) || defined(TARGET_LINUX)
	unsigned int len = strlen(url) + 100;
	char* cmd = malloc(len);
	int ret = snprintf(
	    cmd, len,
		#ifdef TARGET_APPLE
			"open \"%s\"",
		#else
		    "setsid xdg-open \"%s\"",
		#endif
	    url
	);
	if (ret > 0 && ret < len) {
		system(cmd);
	}
	free(cmd);
#else
	#error "AppOpenUrl(...) Not Implemented For Target"
#endif
}
