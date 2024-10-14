#include <iostream>
#include <map>

/*
TO DO:
    3. Decide on the triangles implementation: wheter they should have only one orientation, or all 4 (or more) possibilities
    4. Check all cases :)
*/

class Point {
private:
	int x;
	int y;
public:
	Point() : x(0), y(0) {}
	Point(int p_x, int p_y) : x(p_x), y(p_y) {}

	bool operator==(const Point& other) {
		return (this->x == other.x) && (this->y == other.y);
	}

	bool operator<(const Point& other) const {
		if (this->x == other.x) {
			return this->y < other.y;
		}
		return this->x < other.x;
	}

	bool is_negative() {
		return this->get_x() < 0 || this->get_y() < 0;
	}

	int get_x() const {
		return this->x;
	}

	int get_y() const {
		return this->y;
	}

	void set_x(int x) {
		this->x = x;
	}

	void set_y(int y) {
		this->y = y;
	}
};

class Canvas {
private:
	std::map<Point, char> canva;
public:
	void addTile(Point coordinates, char color) {
		if(canva[coordinates] == color || coordinates.is_negative()) {
			return;
		}
		canva[coordinates] = color;
		printCanvas();
	}

	void printCanvas() {
		int max_x = 0;
		int max_y = 0;

		for(const auto& tile : canva) {
			const Point& p = tile.first;
			if (p.get_x() > max_x) {
				max_x = p.get_x();
			}
			if (p.get_y() > max_y) {
				max_y = p.get_y();
			}
		}

		for(int y = max_y; y >= 0; y--) {
			for(int x = 0; x <= max_x; x++) {
				auto current = Point(x, y);
				if(canva.find(current) != canva.end()) {
					std::cout << canva[current];
				}
				else {
					std::cout << ' ';
				}
			}
			std::cout << std::endl;
		}
		std::cout << "-----------------------------------" << std::endl;
	}
};

class Line {
private:
	Point start;
	Point end;
public:
	Line() : start(Point(0, 0)), end(Point(0, 0)) {}
	Line(Point first, Point second) : start(first), end(second) {}

	void set_start(Point point) {
		this->start = point;
	}

	Point get_start() {
		return this->start;
	}

	void set_end(Point point) {
		this->end = point;
	}

	Point get_end() {
		return this->end;
	}

	bool is_point() {
		return start == end;
	}
};

class Figure {
protected:
	Point position;
	unsigned int width;
	unsigned int height;
	char color;
public:
	virtual void set_height(unsigned int height) {
		this->height = height;
	}

	virtual void set_width(unsigned int width) {
		this->width = width;
	}

	unsigned int get_height() {
		return this->height;
	}

	unsigned int get_width() {
		return this->width;
	}

	void set_color(char c) {
		this->color = c;
	}

	char get_color() const {
		return this->color;
	}

	Point get_position() {
		return this->position;
	}

	void move(Line vector) {
		if(vector.is_point()) {
			return;
		}
		int dx = vector.get_end().get_x() - vector.get_start().get_x();
		int dy = vector.get_end().get_y() - vector.get_start().get_y();
		this->position.set_x(this->position.get_x() + dx);
		this->position.set_y(this->position.get_y() + dy);
	}

	virtual void draw(Canvas& canvas) = 0;
	virtual float get_area() = 0;
};

class Square : public Figure {
public:
	Square() {}
	Square(unsigned int size, char color) {
		this->width = size;
		this->height = size;
		this->color = color;
	}

	float get_area() {
		return this->get_width() * this->get_height();
	}

	void set_width(unsigned int width) override {
		this->width = width;
		this->height = width;
	}

	void set_height(unsigned int height) override {
		this->width = height;
		this->height = height;
	}

	void draw(Canvas& canvas) override {
		int x = this->position.get_x();
		int y = this->position.get_y();
		for(int i = 0; i < this->get_width(); i++) {
			canvas.addTile(Point(x + i, y), this->get_color());
			canvas.addTile(Point(x + i, y + this->get_width() - 1), this->get_color());
			canvas.addTile(Point(x, y + i), this->get_color());
			canvas.addTile(Point(x + this->get_width() - 1, y + i), this->get_color());
		}
	}
};

class Rectangle : public Figure {
	float get_area() {
		return this->get_width() * this->get_height();
	}

	void draw(Canvas& canvas) override {
		int x = this->position.get_x();
		int y = this->position.get_y();
		for (int i = 0; i < this->get_width(); i++) {
			canvas.addTile(Point(x + i, y), this->get_color());
			canvas.addTile(Point(x + i, y + this->get_height() - 1), this->get_color());
		}

		for (int i = 0; i < this->get_height(); i++) {
			canvas.addTile(Point(x, y + i), this->get_color());
			canvas.addTile(Point(x + this->get_width() - 1, y + i), this->get_color());
		}
	}
};

class QuarterSquareTriangle : public Figure {
	float get_area() {
		return this->get_width() * this->get_height() * 0.5;
	}

	void set_width(unsigned int width) override {
		this->width = width;
		this->height = width;
	}

	void set_height(unsigned int height) override {
		this->width = height;
		this->height = height;
	}

	void draw(Canvas& canvas) override {
		int x = this->position.get_x();
		int y = this->position.get_y();
		for(int i = 0; i < this->get_width(); i++) {
			canvas.addTile(Point(x + i, y), this->get_color());
		}
		for(int i = 0; i < this->get_width() / 2 + 1; i++) {
			canvas.addTile(Point(x + i, y - i), this->get_color());
		}
		for(int i = this->get_width() / 2 + 1; i < this->get_width(); i++) {
			canvas.addTile(Point(x + i, y - 2 * (this->get_width() / 2) + i), this->get_color());
		}
	}
};

class HalfSquareTriangle : public Figure {
	float get_area() {
		return this->get_width() * this->get_height() * 0.5;
	}

	void set_width(unsigned int width) override {
		this->width = width;
		this->height = width;
	}

	void set_height(unsigned int height) override {
		this->width = height;
		this->height = height;
	}

	void draw(Canvas& canvas) override {
		int x = this->position.get_x();
		int y = this->position.get_y();
		for(int i = 0; i < this->get_width(); i++) {
			canvas.addTile(Point(x + i, y), this->get_color());
			canvas.addTile(Point(x, y + i), this->get_color());
			canvas.addTile(Point(x + i, y + this->get_width() - i - 1), this->get_color());
		}
	}
};

int main() {
// 	Figure* f = new Square();
// 	f->set_width(5);
// 	f->set_height(10);

// 	// Initial position
// 	Point old_position = f->get_position();
// 	std::cout << "Initial position: " << old_position.get_x() << ", " << old_position.get_y() << std::endl;

// 	// Create a translation vector (Line)
// 	Point start(0, 0);
// 	Point end(3, 4);  // Moving by (3, 4)
// 	Line translation(start, end);

// 	// Move the figure by the vector
// 	f->move(translation);

// 	// After moving, you could inspect the new position if needed
// 	Point new_position = f->get_position();
// 	std::cout << "New position: (" << new_position.get_x() << ", " << new_position.get_y() << ")" << std::endl;

// 	return 0;
	// Figure* f = new Square();
	// f->set_width(5);
	// std::cout << f->get_height() << ", " << f->get_width() << std::endl;
	// f->set_height(102);
	// std::cout << f->get_height() << ", " << f->gets_width() << std::endl;
	Canvas* canvas = new Canvas();

	Figure* f = new QuarterSquareTriangle();
	f->set_color('!');
	f->set_width(9);
	f->move(Line(Point(), Point(5, 5)));
	f->draw(*canvas);

// 	Figure* f2 = new Rectangle();
// 	f2->set_color('%');
// 	f2->set_width(8);
// 	f2->set_height(12);
// 	f2->move(Line(Point(), Point(-2, -5)));
// 	f2->draw(*canvas);
}
