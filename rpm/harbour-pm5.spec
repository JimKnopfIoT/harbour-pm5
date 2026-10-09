Name:       harbour-pm5

# >> macros
%define _buildhost reproducible-builder
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Proxmark control for SailfishOS
Version:    0.0.1
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        https://github.com/JimKnopfIoT/harbour-pm5
Source0:    %{name}-%{version}.tar.bz2
Vendor:     reproducible-builder
Packager:   harbour-pm5 contributors
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  desktop-file-utils

%description
Drives a Proxmark over USB (CDC-ACM) from the phone: 125 kHz LF badge search
and T5577 write, 13.56 MHz reader, MIFARE Classic key check and dump. Speaks
the device wire protocol directly over the serial port; no client binary.

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%post
systemctl restart sailjaild >/dev/null 2>&1 || true

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
