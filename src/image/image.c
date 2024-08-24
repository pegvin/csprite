#include <string.h>
#include "image/image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "log/log.h"
#include "fs/fs.h"

int ImageInit(image_t* img, uint32_t width, uint32_t height) {
	img->pixels = calloc(width * height, sizeof(pixel_t));
	if (img->pixels == NULL) {
		log_error("Allocation failed");
		return 1;
	}

	img->width = width;
	img->height = height;
	return 0;
}

int ImageInitFrom(image_t* img, const char* filePath) {
	int w = 0, h = 0, c = 0;
	stbi_uc* data = stbi_load(filePath, &w, &h, &c, 4);
	if (data == NULL) {
		log_error("stbi_load(...) returned NULL");
		return 1;
	}
	if (w < 1 || h < 1 || c != 4) {
		log_error("Invalid/Unexpected Image Properties: %dx%dx%d", w, h, c);
		stbi_image_free(data);
		return 1;
	}
	if (ImageInit(img, w, h)) {
		return 1;
	}

	for (long long i = 0; i < w * h; i++) {
		pixel_t* out = &img->pixels[i];
		out->r = data[(i * 4) + 0];
		out->g = data[(i * 4) + 1];
		out->b = data[(i * 4) + 2];
		out->a = data[(i * 4) + 3];
	}

	stbi_image_free(data);

	return 0;
}

int ImageWriteTo(image_t* img, const char* filePath) {
	int extensionIdx = FsGetExtension(filePath);
	if (extensionIdx < 0) {
		log_error("Failed to find extension of '%s'", filePath);
		return 1;
	}

	const char* extension = filePath + extensionIdx;
	if (strcmp(extension, ".png") == 0) {
		stbi_write_png_compression_level = 9;
		stbi_write_png(filePath, img->width, img->height, 4, img->pixels, img->width * sizeof(pixel_t));
	} else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
		stbi_write_jpg(filePath, img->width, img->height, 4, img->pixels, 100);
	} else {
		log_error("Unsupported extension '%s'", extension);
	}

	return 0;
}

void ImageDestroy(image_t* img) {
	free(img->pixels);
	*img = (image_t){0};
}
