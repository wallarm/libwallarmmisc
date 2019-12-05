Name: libwallarmmisc-devel
Version: 0.5.0
Release: 5
Summary: A header-only red-black tree algorithm C library

License: BSD
URL: https://wallarm.com

Source: libwallarmmisc-%{version}.tar.gz

BuildRequires: /usr/bin/gcc
BuildRequires: cmake
BuildRequires: CUnit-devel

Group: Development/Libraries

%description
The library extends FreeBSD's sys/tree.h by adding convenient macroses and
algorithms.

%prep
%setup -c
./config

%build
make -C build

%install
install -d -m 755 $RPM_BUILD_ROOT%{_includedir}/
install -d -m 755 $RPM_BUILD_ROOT%{_includedir}/wallarm
install -d -m 755 $RPM_BUILD_ROOT%{_includedir}/wallarm/sys
install -m 644 build/include/wallarm/*.h $RPM_BUILD_ROOT%{_includedir}/wallarm
install -m 644 include/wallarm/*.h $RPM_BUILD_ROOT%{_includedir}/wallarm
install -m 644 include/wallarm/sys/*.h $RPM_BUILD_ROOT%{_includedir}/wallarm/sys

%files
%{_includedir}/

%changelog
* Thu Dec 05 2019 Dmitry Murzin <dmurzin@wallarm.com> - 0.5.0-5
- Rebuilt for CentOS 8 (NODE-1790)

* Thu Nov 21 2019 Dmitry Murzin <dmurzin@wallarm.com> - 0.5.0-4
- Bump version

* Fri Jul 26 2019 Dmitry Murzin <dmurzin@wallarm.com> - 0.5.0-3
- Added: build for debian buster (Closes: NODE-1977)

* Wed Jul 10 2019 Oleg Trifonov <otrifonov@wallarm.com> - 0.5.0-2
- NODE-1948: Internal rebuild

* Wed May 22 2019 Oleg Trifonov <otrifonov@wallarm.com> - 0.5.0-1
- Updated: W_CEIL_P2 macros added (Closes: NODE-1766)
* Thu Apr 25 2019 Nikolai Miroshnichenko <nmiroshnichenko@wallarm.com> - 0.4.0-1
- Updated: Macroses improvement added (Closes: NODE-1648)
* Mon Apr 01 2019 Nikolai Miroshnichenko <nmiroshnichenkoi@wallarm.com> - 0.3.0-1
- Updated: Utils and wordmask macroses added (Closes: NODE-1564)
* Thu Sep 27 2018 Alexey Temnikov <atemnikov@wallarm.com> 0.2.0-1
- added bsearch (Closes: NODE-1279)
* Mon Feb 12 2018 Alexey Remizov <ar@wallarm.com> 0.1.1-1
- Fixed dependencies.
* Fri Feb 9 2018 Alexey Remizov <ar@wallarm.com> 0.1.0-1
- Initial release
