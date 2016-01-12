#version 330

uniform vec4 colour;

out vec4 fragColor;

void main() {
	fragColor = vec4( colour );
}
