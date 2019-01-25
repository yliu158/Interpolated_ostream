#include <stdexcept>
#include <iostream>
#include <cstring>
#include <sstream>
#include <stdexcept>

std::ostream& (&ffr(std::ostream&(&f)(std::ostream&)))(std::ostream&){
	return f;
}

namespace cs540 {
	struct WrongNumberOfArgs: public std::invalid_argument {};

	template<typename ...Ts>
	struct Ref {};

	template<typename T>
	struct Ref<T> {
		const T* arg; std::string pre; std::string other;
		Ref<T> (const T& t) : arg(t){pre = "";}
		Ref<T> () {pre = "";}
		friend std::ostream& operator<< (std::ostream& os, const Ref& r) {
			if (r.pre != "") os << r.pre;
			if (r.arg) os << *r.arg;
			os << r.other;
			return *&os;
		}
	};
	template <typename T, typename ...Ts>
	struct Ref<T, Ts...> {
		const T* arg; std::string pre;
		Ref<Ts...> other;
		Ref<T, Ts...> (const T& t) : arg(t){pre = "";}
		Ref<T, Ts...> () {pre = "";}
		Ref(const Ref& r): arg(r.arg) {pre = r.pre;}
		friend std::ostream& operator<< (std::ostream& os, const Ref& r) {
			if (r.pre != "") os << r.pre;
			os << *r.arg << r.other;
			return *&os;
		}
	};

	const std::string Interpolate(const char *c_arr) {
		std::string pre = "";
		for (size_t i = 0; i < strlen(c_arr); i++) {
			if (c_arr[i] == '\\' && c_arr[i+1] == '%') {pre += '%'; ++i; continue;}
			else if (c_arr[i] == '%') throw std::invalid_argument("WrongNumberOfArgs");
			else {pre += c_arr[i];}
		}
		return pre;
	}

	template <typename T>
	const Ref<T> Interpolate(const char* c_arr, const T& t) {
		Ref<T> r;
		std::ostringstream os; os << t;
		std::string replace = os.str();
		r.arg = &t;
		if (replace == "") {r.other = Interpolate(c_arr); return r;}
		size_t i = 0; bool f = false;
		for (; i < strlen(c_arr); ++i) {
			if (c_arr[i] == '\\' && c_arr[i+1] == '%') {r.pre += '%'; ++i; continue;}
			if (c_arr[i] == '%') {
				r.other = Interpolate(c_arr+i+1); f = true;
				break;
			} else {
				r.pre += c_arr[i];
			}
		}
		if (!f) {throw std::invalid_argument("WrongNumberOfArgs");}
		r.other = Interpolate(c_arr+i+1);
		return r;
	}

	template <typename T, typename ... Ts>
	const Ref<T, Ts...> Interpolate(const char* c_arr, const T& t, const Ts& ... ts) {
		Ref<T, Ts...> r;
		std::ostringstream os; os << t;
		std::string replace = os.str();
		r.arg = &t;
		if (replace == "") {r.other = Interpolate(c_arr, ts...); return r;}
		std::string s = ""; bool f = false;
		size_t i = 0;
		for (; i < strlen(c_arr); ++i) {
			if (c_arr[i] == '\\' && c_arr[i+1] == '%') {s += '%'; ++i; continue;}
			if (c_arr[i] == '%') {
				r.pre = s; f = true;
				break;
			} else {
				s += c_arr[i];
			}
		}
		if (!f) {throw std::invalid_argument("WrongNumberOfArgs");}
		r.other = Interpolate(c_arr+i+1, ts...);
		return r;
	}


}
