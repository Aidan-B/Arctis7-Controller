#pragma once

// Credit Nevermore on stack overflow: https://stackoverflow.com/a/32763779/11074982

#include <chrono>
#include <sys/time.h>


namespace std {
namespace chrono {
namespace detail {

template<typename From, typename To>
struct posix_duration_cast;

// chrono -> timeval caster
template<typename Rep, typename Period>
struct posix_duration_cast< std::chrono::duration<Rep, Period>, struct timeval > {

    static struct timeval cast(std::chrono::duration<Rep, Period> const& d) {
        struct timeval tv;

        std::chrono::seconds const sec = std::chrono::duration_cast<std::chrono::seconds>(d);

        tv.tv_sec  = sec.count();
        tv.tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(d - sec).count();

        return std::move(tv);
    }

};

// timeval -> chrono caster
template<typename Rep, typename Period>
struct posix_duration_cast< struct timeval, std::chrono::duration<Rep, Period> > {

    static std::chrono::duration<Rep, Period> cast(struct timeval const & tv) {
        return std::chrono::duration_cast< std::chrono::duration<Rep, Period> >(
                    std::chrono::seconds(tv.tv_sec) + std::chrono::microseconds(tv.tv_usec)
                    );
    }

};

}

// chrono -> timeval    
template<typename T, typename Rep, typename Period>
auto duration_cast(std::chrono::duration<Rep, Period> const& d)
-> std::enable_if_t< std::is_same<T, struct timeval>::value, struct timeval >
{
    return detail::posix_duration_cast< std::chrono::duration<Rep, Period>, timeval >::cast(d);
}

// timeval -> chrono
template<typename Duration>
Duration duration_cast(struct timeval const& tv) {
    return detail::posix_duration_cast< struct timeval, Duration >::cast(tv);
}

} // chrono
} // std
