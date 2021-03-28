#ifndef EXT_HPP
#define EXT_HPP
#include <limits>
#include <list>
#include <memory>

template<class Node>
struct helper
{
    void visit(bool visit) { visited = visit; }
    bool is_visited() const {return visited; }
    void clear()
    {
        visited = false;
    }
private:
    bool visited = false;
};


struct colored_timed_helper
{
    enum class COLOR
    {
        WHITE,
        GRAY,
        BLACK
    };

    colored_timed_helper() = default;

    void visit(size_t time)
    {
        assert(color == COLOR::WHITE && "Visit with not WHITE");
        color = COLOR::GRAY;
        d = time;
    }
    void finish(size_t time)
    {
        assert(color == COLOR::GRAY && "Finish with not GRAY");
        color = COLOR::BLACK;
        f = time;
        assert(d <= f && "Visited must happens before finished");
    }

    bool is_not_visited() const { return color == COLOR::WHITE; }
    bool is_visited() const {return (color == COLOR::GRAY/* or color == COLOR::BLACK*/); }
    bool is_finished() const { return color == COLOR::BLACK; };
    void clear()
    {
        color = COLOR::WHITE;
        d = 0;
        f = 0;
    }
    size_t get_d() const { return d;}
    size_t get_f() const { return f;}
private:
    COLOR color = COLOR::WHITE;
    size_t d = 0;
    size_t f = 0;
};



template<class Node>
struct timed_predesc_helper : colored_timed_helper
{
    using base_t = colored_timed_helper;
    using weak_ptr_t = std::weak_ptr<Node>;
    using strong_ptr_t = std::shared_ptr<Node>;

    void set_predescessor(strong_ptr_t pred)
    {
        assert(!get_predescessor() && "Predescessor set already");
        predescessor = pred;
    }

    const strong_ptr_t get_predescessor() const
    {
        return predescessor.lock();
    }

    void clear()
    {
        base_t::clear();
        weak_ptr_t().swap(predescessor);
    }
private:
    weak_ptr_t predescessor;
};


template<class Node>
struct shortest_path_helper : timed_predesc_helper<Node>
{
    using base_t = timed_predesc_helper<Node>;
    using estimate_t = size_t;

    void set_sp_estimate(estimate_t est)
    {
        shortest_path_estimate = est;
    }

    const estimate_t get_sp_estimate() const
    {
        return shortest_path_estimate;
    }

    void clear()
    {
        base_t::clear();
        shortest_path_estimate = std::numeric_limits<estimate_t>::max();
    }
private:
    size_t shortest_path_estimate;
};


template<class T>
struct payload
{
    using value_t = T;
    explicit payload(value_t v ) : value (std::move(v)) {}
    value_t value;
};




template<class Helper>
struct aux_storage
{
    using helper_t = Helper;

    template<class ...Args>
    std::shared_ptr<helper_t> create_aux_data(Args&& ...args)
    {
        auto aux = std::make_shared<helper_t>(std::forward<Args>(args)...);
        storage.push_back(aux);
        return aux;
    }

    void clear()
    {
        for(auto it = storage.begin(); it != storage.end(); )
        {
            auto lock = it->lock();
            if(!lock)
            {
                it = storage.erase(it);
                continue;
            }

            lock->clear();
            ++it;
        }
    }
private:
    std::list<std::weak_ptr<helper_t>> storage;
};
#endif //EXT_HPP
