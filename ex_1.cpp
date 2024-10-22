#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>

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

	unsigned int get_x() {
		return this->x;
	}

	unsigned int get_y() {
		return this->y;
	}

	void set_x(int x) {
		this->x = x;
	}

	void set_y(int y) {
		this->y = y;
	}
};

class Line {
private:
	Point start;
	Point end;
	char color;
public:
	Line() : start(Point(0, 0)), end(Point(0, 0)), color(' ') {}
	Line(Point first, Point second) : start(first), end(second), color(' ') {}
	Line(Point first, Point second, char color) : start(first), end(second), color(color) {}

	void set_start(Point point) {
		this->start = point;
	}
	
	void set_color(char c) {
	    this->color = c;
	}
	
	char get_color() const {
	    return this->color;
	}

	Point get_start() const {
		return this->start;
	}

	void set_end(Point point) {
		this->end = point;
	}

	Point get_end() const {
		return this->end;
	}

	bool is_point() {
		return start == end;
	}
};

class Canvas {
private:
    std::vector<Line> lines;
public:
    void printGrid(std::vector<std::vector<char>> grid, unsigned int max_x, unsigned int max_y) {
        for(int y = max_y; y >= 0; y--) {
            for(int x = 0; x <= max_x; x++) {
                std::cout << grid[y][x];
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------------------" << std::endl;
    }
    
    void addLine(Line line) {
        lines.push_back(line);
    }
    
    void printCanvas() {
        unsigned int max_x = 0;
        unsigned int max_y = 0;
        
        for(const Line& line : lines) {
            max_x = std::max(max_x, std::max(line.get_start().get_x(), line.get_end().get_x()));
            max_y = std::max(max_y, std::max(line.get_start().get_y(), line.get_end().get_y()));
        }
        
        std::vector<std::vector<char>> grid(max_y + 1, std::vector<char>(max_x + 1, ' '));
        
        for(const Line& line : lines) {
            Point start = line.get_start();
            Point end = line.get_end();
            
            // vertical
            if(start.get_x() == end.get_x()) {
                int minY = std::min(start.get_y(), end.get_y());
                int maxY = std::max(start.get_y(), end.get_y());
                
                for(int y = minY; y <= maxY; y++) {
                    if(y >= 0) {
                        grid[y][start.get_x()] = line.get_color();   
                    }
                }
            }
            // diagonal
            else if(start.get_y() == end.get_y()) {
                int minX = std::min(start.get_x(), end.get_x());
                int maxX = std::max(start.get_x(), end.get_x());
                
                for(int x = minX; x <= maxX; x++) {
                    if(x >= 0) {
                        grid[start.get_y()][x] = line.get_color();   
                    }
                }
            }
            // 45 degree
            else {
                int x1 = start.get_x();
                int x2 = end.get_x();
                int y1 = start.get_y();
                int y2 = end.get_y();
                
                int dx = std::abs(x1 - x2);
                int dy = std::abs(y1 - y2);
                
                if(dx != dy) {
                    throw std::runtime_error("Invalid line");
                }
                
                int sx = (x1 < x2) ? 1 : -1;
                int sy = (y1 < y2) ? 1 : -1;
                
                while(x1 != x2 || y1 != y2) {
                    grid[y1][x1] = line.get_color();
                    x1 += sx;
                    y1 += sy;
                }
                grid[y1][x1] = line.get_color();
            }
        }
        
        this->printGrid(grid, max_x, max_y);
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
        unsigned int size = this->get_width();
    
        Line top_edge(Point(x, y), Point(x + size - 1, y), this->get_color());
        Line bottom_edge(Point(x, y + size - 1), Point(x + size - 1, y + size - 1), this->get_color());
        Line left_edge(Point(x, y), Point(x, y + size - 1), this->get_color());
        Line right_edge(Point(x + size - 1, y), Point(x + size - 1, y + size - 1), this->get_color());

        canvas.addLine(top_edge);
        canvas.addLine(bottom_edge);
        canvas.addLine(left_edge);
        canvas.addLine(right_edge);
        
        canvas.printCanvas();
    }

};

class Rectangle : public Figure {
	float get_area() {
		return this->get_width() * this->get_height();
	}

	void draw(Canvas& canvas) override {
        int x = this->position.get_x();
        int y = this->position.get_y();
        unsigned int size = this->get_width();
    
        Line top_edge(Point(x, y), Point(x + size - 1, y), this->get_color());
        Line bottom_edge(Point(x, y + size - 1), Point(x + size - 1, y + size - 1), this->get_color());
        Line left_edge(Point(x, y), Point(x, y + size - 1), this->get_color());
        Line right_edge(Point(x + size - 1, y), Point(x + size - 1, y + size - 1), this->get_color());
        
        canvas.addLine(top_edge);
        canvas.addLine(bottom_edge);
        canvas.addLine(left_edge);
        canvas.addLine(right_edge);
        
        canvas.printCanvas();
    }
};

class Triangle : public Figure {
public:
    void draw_triangle(Canvas& canvas, const Point& a, const Point& b, const Point& c) {
        canvas.addLine(Line(a, b, this->get_color()));
        canvas.addLine(Line(a, c, this->get_color()));
        canvas.addLine(Line(b, c, this->get_color()));
    }
};

class QuarterSquareTriangle : public Triangle {
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
        unsigned int width = this->get_width() - 1;
        unsigned int height = this->get_width() / 2;
        
        Point a = this->get_position();
        Point b = Point(a.get_x() + width, a.get_y());
        Point c = Point(a.get_x() + height, a.get_y() - height);
        
        this->draw_triangle(canvas, a, b, c);
        canvas.printCanvas();

	}
};

class HalfSquareTriangle : public Triangle {
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
        unsigned int width = this->get_width() - 1;
        
        Point a = this->get_position();
        Point b = Point(a.get_x(), a.get_y() - width);
        Point c = Point(a.get_x() + width, a.get_y() - width);
        
        this->draw_triangle(canvas, a, b, c);
        canvas.printCanvas();

	}
};