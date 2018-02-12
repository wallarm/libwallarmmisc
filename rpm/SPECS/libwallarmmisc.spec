Name: libwallarmmisc-devel
Version: 0.1.1
Release: 1
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
* Mon Feb 12 2018 Alexey Remizov <ar@wallarm.com> 0.1.1-1
- Fixed dependencies.
* Fri Feb 9 2018 Alexey Remizov <ar@wallarm.com> 0.1.0-1
- Initial release
