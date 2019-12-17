# Quick start

To compile the code simply type:

	make

You can add "-j5" in order to parallelise the compilation:

	make -j5

Then to execute the program, you can execute the following command:

	./main

This command will select by the default the scene described in the scene.json file.If you want to render another scene simply execute:

	./main -s=<path_to_my_file.json>

You can also use `--scene` instead of `-s`

You can also specifify the number of threads you want to use. Just add the argument `-t=<num_threads>` or `--threads=<num_threads>`. By default, the program uses all the threads.

# Defining a scene

To define a scene, you need to make a json file describing the scene. The structure of this json file is the following:

```javascript
{
	// Array of all your lights
	"lights": [
		{
			"type": "spot or sun",
			"position": "a b c",
			"color": "r g b"
		}
	],
	"objects": [
		{
			"type": "obj, triangle or sphere",
			"color": "r g b",
			"opacity": 1,
			"reflexitivity": 0,
			"translation": "a b v",
			"rotation": "0 180 360"
			// if obj
			"path": "paht to file obj",
			// if triangle
			"positions": [
				"a b c",
				"a b c",
				"a b c"
			],
		},
	],
	"camera": {
		"position": "a b c",
		"direction": "a b c",
		"dist_normal": 1 // distance to the screen
	},
	"engine": {
		"screen_height": 1024,
		"screen_width": 1024,
		"field_of_vue": 70,
		"top_dir": "a b c",
 		// Array of pixels offsets, it shoots the ray considering those pixels offets
		"pixels_offset": [
			"0.5 0.5", // A ray is shot from the center of each pixel
			"0.75 0.75" // A ray is shot from the bottom right corner at 75 %
		],
		"recursion_level": 3, // number of reflexion or transmission for 1 ray
		"env_diffusion": 0.5, // Environnmental diffusion
		"output_name": "results/chair" // Output name file
	}
}
```
