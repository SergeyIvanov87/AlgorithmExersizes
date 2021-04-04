#ifndef TRACER
#define TRACER
#include <sstream>
#include <utility>

template<class Iterator>
struct ranged
{
    using value_type = typename Iterator::value_type;
    Iterator beg, end;
};

template<class It>
ranged<It> MakeIncludingRange(It begin, It end)
{
    return ranged<It> {begin, std::next(end)};
}

template<char ident = ' '>
struct StackedPolicy
{
    void increaseDepth(const StackedPolicy&src)
    {
        m_prefix = src.m_prefix;
        m_prefix.push_back(ident_value());
    }

    void decreaseDepth()
    {
        if(!m_prefix.empty())
        {
            m_prefix.pop_back();
        }
    }

    const std::string& getPrefix() const
    {
        return m_prefix;
    }
private:
    std::string m_prefix;
    static constexpr const char ident_value()
    {
        return ident;
    }
};


struct EmptyPolicy
{
    void increaseDepth(const EmptyPolicy&src)
    {
    }

    void decreaseDepth()
    {
    }

    const std::string& getPrefix() const
    {
        static const std::string empty;
        return empty;
    }
};


template<class Impl, class Policy>
struct TracerBase : Policy
{
    TracerBase() = default;
    TracerBase(TracerBase&&src) = default;
    TracerBase& operator=(TracerBase&&src) = default;

    using self_t = TracerBase<Impl, Policy>;

    TracerBase(TracerBase& src)
    {
        Policy::increaseDepth(src);
    }

    TracerBase& operator=(TracerBase&src)
    {
        Policy::increaseDepth(src);
        return static_cast<Impl&>(*this);
    }

    ~TracerBase()
    {
        Policy::decreaseDepth();
    }

    Impl& self() noexcept
    {
        return const_cast<Impl&>(static_cast<const self_t*>(this)->self());
    }

    const Impl& self() const noexcept
    {
         return *static_cast<const Impl*>(this);
    }

    template <class T>
    Impl &operator<< (const T &val)
    {
        self().trace_impl(Policy::getPrefix(), val);
        return self();
    }

    Impl& operator<<(
    std::ios_base& (*func)(std::ios_base&))
    {
        self().trace_impl(func);
        return self();
    }

    Impl &operator<< (
    std::ostream& (*func)( std::ostream& ) )
    {
        self().trace_impl(func);
        return self();
    }

    template<class ...T>
    void trace(const T& ...val)
    {
        self().trace_impl(Policy::getPrefix(), val...);
        *this << std::endl;
    }

    /*
    Impl& operator<<(
    Impl& (*func)(Impl&) )
    {
    }*/
};

struct StdoutTracer : public TracerBase<StdoutTracer, StackedPolicy<' '>>
{
    using BaseType = TracerBase<StdoutTracer, StackedPolicy<' '>>;
    using BaseType::operator <<;

    template<class ...T>
    void trace_impl(T&& ...val)
    {
        (std::cout << ... << val);
    }

    template<class Iterator>
    void trace_impl(const std::string& pref, const ranged<Iterator>& val)
    {
        std::cout << pref;
        std::copy(val.beg, val.end, std::ostream_iterator<int>(std::cout, " "));
    }
};

struct EmptyTracer: public TracerBase<EmptyTracer, EmptyPolicy>
{
    using BaseType = TracerBase<EmptyTracer, EmptyPolicy>;
    using BaseType::operator <<;

    template<class ...T>
    void trace_impl(T&& ...val)
    {
    }
};

#endif
