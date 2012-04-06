#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

template<class T, class U, class V, class W>
pair<T, U> operator + (pair<T, U> a, pair <V, W> b) {
	return pair<T, U>(a.first + b.first, a.second + b.second);
}

template<class T, class U, class V, class W>
pair<T, U> operator - (pair<T, U> a, pair <V, W> b) {
	return pair<T, U>(a.first - b.first, a.second - b.second);
}

template<class T, class U>
ostream& operator << (ostream& out, pair<T, U>& a) {
	out << "(" << a.first << " " << a.second << ")";
	return out;
}

/*!
 * Class for generating eggbot commands from a given function.
 */
template<class T>
class Manifold {
	public:
	
	Manifold() {
		position.first = 0;
		position.second = 0;
	}

	/*!
	 * Open the output file which will be filled with eggbot commands.
	 */
	void open(const char * filename) {
		out.close();
		out.open(filename, fstream::out);
	}

	/*!
	 * Plot a given function
	 */
	template<class U>
	void plot(U& func, bool backwards = false) {
		const pair<T, T> startpos = get_val(func, 0, backwards);
		const pair<int, int> discrete = position;
		T t = 0;
		T dt = 0.1;
		for (;;) {
			pair<T, T> new_pos = get_val(func, t + dt, backwards) - startpos + discrete;
			if (discrete_distance(new_pos, position) == 0) {
				dt *= 1.1;
				continue;
			}
			if (discrete_distance(new_pos, position) > 1) {
				dt *= .9;
				continue;
			}
			move(new_pos);
			t += dt;
			if (t >= 1) {
				new_pos = get_val(func, 1, backwards) - startpos + discrete;
				move(new_pos);
				return;
			}
		}
	}

	template<class U>
	pair<T, T> get_val(U& func, T t, bool backwards) {
		if (!backwards) {
			return func(t);
		}
		else {
			return func(1.0-t);
		}
	}

	template<class U, class V>
	void move(pair<U, V> new_pos) {
		const int new_x = (int)round(new_pos.first);
		const int new_y = (int)round(new_pos.second);
		bool up = false, down = false, left = false, right = false;
		if (new_x > position.first) {
			right = true;
		}
		else if (new_x < position.first) {
			left = true;
		}
		if (new_y > position.second) {
			down = true;
		}
		else if (new_y < position.second) {
			up = true;
		}

		position.first = new_x;
		position.second = new_y;

		if (left && up) {
			out << '1';
			return;
		}
		if (right && up) {
			out << '2';
			return;
		}
		if (left && down) {
			out << '3';
			return;
		}
		if (right && down) {
			out << '4';
			return;
		}
		if (left) {
			out << 'l';
			return;
		}
		if (right) {
			out << 'r';
			return;
		}
		if (up) {
			out << 'u';
			return;
		}
		if (down) {
			out << 'd';
			return;
		}
	
	}

	// Calculate the infinity distance between two points
	template<class U, class V, class W, class X>
	int discrete_distance(pair<U, V> a, pair<W, X> b) {
		return (int)max(fabs(round(a.first) - round(b.first)), fabs(round(a.second)-round(b.second)));
	}

	private:
	fstream out;

	pair<int, int> position;

};

template<class T>
class Circle {
	public:

	Circle() {
		radius = 100;
	}

	/*!
	 * Generate a circle
	 */
	pair<T, T> operator() (T t) {
		pair<T, T> point;
		point.first = cos(2*M_PI*t)*radius;
		point.second = sin(2*M_PI*t)*radius;
		return point;
	}

	void set_radius(T rad) {
		radius = rad;
	}

	T radius;
};

template<class T>
class Spiral {
	public:

	Spiral() {
		scale = 1000;
		mirror_x = false;
		mirror_y = false;
	}

	/*!
	 * Generate a logarithmic spiral
	 */
	pair<T, T> operator() (T t) {
		pair<T, T> point;
		T k = -0.05;
		t *= 15*2*M_PI;
		point.first  = (mirror_y ? -1:1)*exp(k*t)*cos(t)*scale;
		point.second = (mirror_x ? -1:1)*exp(k*t)*sin(t)*scale;
		return point;
	}

	T scale;
	bool mirror_x;
	bool mirror_y;
};


int main(void) {

	typedef double T;

	Manifold<T> man;
	Circle<T> circ;

	man.open("circ.egg");

	circ.radius = 30;
	while (circ.radius < 1000) {
		cout << "Plotting Circle" << endl;
		man.plot(circ);
		circ.radius *= 1.2;
	}

	man.open("spiral.egg");
	Spiral<T> spiral;
	man.plot(spiral, true);
	spiral.mirror_y = true;
	man.plot(spiral);

	man.open("spiral2.egg");
	spiral.mirror_y = false;
	spiral.mirror_x = false;
	man.plot(spiral,true);
	spiral.mirror_y = true;
	spiral.mirror_x = true;
	man.plot(spiral);


	return 0;
}

