Name:       curl
Summary:    A utility for getting files from remote servers (FTP, HTTP, and others)
Version:    7.21.3
Release:    7
Group:      Applications/Internet
License:    MIT
#URL:        http://curl.haxx.se/
#Source0:    http://curl.haxx.se/download/%{name}-%{version}.tar.bz2
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(libidn)
BuildRequires:  pkgconfig(nss)
BuildRequires:  pkgconfig(zlib)
BuildRequires:  pkgconfig(libcares)
Provides:   webclient


%description
cURL is a tool for getting files from HTTP, FTP, FILE, LDAP, LDAPS,
DICT, TELNET and TFTP servers, using any of the supported protocols.
cURL is designed to work without user interaction or any kind of
interactivity. cURL offers many useful capabilities, like proxy support,
user authentication, FTP upload, HTTP post, and file transfer resume.



%package -n libcurl
Summary:    A library for getting files from web servers
Group:      System/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libcurl
This package provides a way for applications to use FTP, HTTP, Gopher and
other servers for getting files.


%package -n libcurl-devel
Summary:    Files needed for building applications with libcurl
Group:      Development/Libraries
Requires:   libcurl = %{version}-%{release}
Requires:   libidn-devel
Provides:   curl-devel = %{version}-%{release}
Obsoletes:   curl-devel < %{version}-%{release}

%description -n libcurl-devel
cURL is a tool for getting files from FTP, HTTP, Gopher, Telnet, and
Dict servers, using any of the supported protocols. The libcurl-devel
package includes files needed for developing applications which can
use cURL's capabilities internally.



%prep
%setup -q


%build

export CPPFLAGS="$(pkg-config --cflags nss) -DHAVE_PK11_CREATEGENERICOBJECT"

%reconfigure --without-nss --without-gnutls --with-openssl --disable-ipv6 \
--with-ca-path=/etc/ssl/certs \
--with-libidn \
--with-lber-lib=lber \
--enable-manual --enable-versioned-symbols --enable-ares --enable-debug --enable-curldebug \
--disable-static

#--with-ca-bundle=%{_sysconfdir}/pki/tls/certs/ca-bundle.crt 

sed -i -e 's,-L/usr/lib ,,g;s,-L/usr/lib64 ,,g;s,-L/usr/lib$,,g;s,-L/usr/lib64$,,g' \
Makefile libcurl.pc
# Remove bogus rpath
sed -i \
-e 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' \
-e 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' libtool

make %{?_smp_mflags}

%install
rm -rf %{buildroot}

rm -rf $RPM_BUILD_ROOT

make DESTDIR=$RPM_BUILD_ROOT INSTALL="%{__install} -p" install

rm -f ${RPM_BUILD_ROOT}%{_libdir}/libcurl.la
install -d $RPM_BUILD_ROOT/%{_datadir}/aclocal
install -m 644 docs/libcurl/libcurl.m4 $RPM_BUILD_ROOT/%{_datadir}/aclocal


# don't need curl's copy of the certs; use openssl's
find ${RPM_BUILD_ROOT} -name ca-bundle.crt -exec rm -f '{}' \;
rm -rf ${RPM_BUILD_ROOT}/usr/share/man

%post -n libcurl -p /sbin/ldconfig

%postun -n libcurl -p /sbin/ldconfig

%files
%{_bindir}/curl

%files -n libcurl
%{_libdir}/libcurl.so.*

%files -n libcurl-devel
%{_bindir}/curl-config*
%{_includedir}/curl
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc
%{_datadir}/aclocal/libcurl.m4

