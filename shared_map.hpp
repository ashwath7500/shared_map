#include <memory>
#include <unordered_map>
#include <unordered_set>
using namespace std;

constexpr int max_depth = 5;

template<typename key, typename value_t>
struct shared_map;

template<typename key, typename value_t>
struct map_object{
	typedef shared_map<key, value_t> smap;
	value_t ele;
	key ki;
	smap* map_p;
	operator value_t() const {
		return ele;
	}
	const value_t& operator=(const value_t& val) {
		map_p->insert(ki,val);
		return val;
	}
};

template<typename key, typename value_t>
struct fragment{
	shared_ptr<fragment> parent;
	unordered_map<key, value_t> u_map;
	unordered_set<key> deleted_keys;
	fragment absolute_fragment() {
		fragment ret;
		if (parent) ret = parent->absolute_fragment();
		for (auto& it : deleted_keys) ret.u_map.erase(it);
		for (auto& it : u_map) ret.u_map.insert(it);
		return ret;
	}
 	value_t find(key k) {
		if (u_map.find(k) != u_map.end()) {
			return u_map[k];
		}
        value_t vt;
		if (!parent) return vt;
		return parent->find(k);
	}
	~fragment() {
		cout<<"gone\n";
	}
};

template<typename key, typename value_t>
struct shared_map{
  typedef fragment<key, value_t> frag;
	shared_map() : depth(1) {
		head = make_shared<frag>();
	}
	void erase(key k) {
		if (head.use_count() > 1) this->new_head();
		head->u_map.erase(k);
		head->deleted_keys.insert(k);
	}
	void insert(key k, value_t value) {
		if (head.use_count() > 1) this->new_head();
		head->u_map[k] = value;
	}
	map_object<key, value_t> operator[](key k) {
		map_object<key, value_t> ret;
		ret.ki = k;
		ret.ele = head->find(k);
        ret.map_p = this;
        return ret;
	}
	void new_head() {
		if (depth < max_depth) {
			auto temp = head;
			head = make_shared<frag>();
			head->parent = temp;
			depth++;
		}
		else  {
			head = make_shared<frag>(head->absolute_fragment());
		}
	}
	int depth;
	shared_ptr<frag> head;
};