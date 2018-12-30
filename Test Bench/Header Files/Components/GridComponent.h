#pragma once

struct GridComponent {
	GridComponent() = default;
	GridComponent(int x, int y) : x(x), y(y) {};
	int x, y;
};