AC_INIT([evosugar], [0.0.0])
AC_CONFIG_SRCDIR(src)

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE

dnl make sure we keep ACLOCAL_FLAGS around for maintainer builds to work
AC_SUBST(ACLOCAL_AMFLAGS, "\${ACLOCAL_FLAGS}")

AC_PROG_INTLTOOL([0.21])

AC_PROG_CC
AC_ISC_POSIX
AC_HEADER_STDC
AM_DISABLE_STATIC
AM_PROG_LIBTOOL

GNOME_COMPILE_WARNINGS

dnl base packages and versions
LIBGTK_REQUIRED=2.4.0
LIBGNOME_REQUIRED=2.7.0
LIBGNOMEUI_REQUIRED=2.7.0
EVOLUTION_REQUIRED=2.3.1
GSOAP_REQUIRED=2.7
LIBSSL_REQUIRED=0.9.8

PKG_CHECK_MODULES(TEST_EPLUGIN,
  [libgnome-2.0 >= $LIBGNOME_REQUIRED dnl
   libgnomeui-2.0 >= $LIBGNOMEUI_REQUIRED dnl
   gtk+-2.0 >= $LIBGTK_REQUIRED dnl
   evolution-plugin >= $EVOLUTION_REQUIRED dnl
   gsoap >= $GSOAP_REQUIRED dnl
   libssl >= $LIBSSL_REQUIRED dnl
])

dnl Evolution plugin install directory
AC_ARG_WITH(plugin-install-dir, [  --with-plugin-install-dir=PATH path to evolution plugin directory])
if test "x$with_plugin_install_dir" = "x" ; then
   PLUGIN_INSTALL_DIR=`pkg-config --variable=plugindir evolution-plugin`
   if test "x$PLUGIN_INSTALL_DIR" = "x"; then
      AC_MSG_ERROR(Unable to find plugin directory)
   fi
fi
AC_SUBST(PLUGIN_INSTALL_DIR)

dnl Evolution e-error install directory
ERROR_DIR=`pkg-config --variable=errordir evolution-plugin`
if test "x$ERROR_DIR" = "x"; then
   AC_MSG_ERROR(Unable to find error file directory)
fi
AC_SUBST(ERROR_DIR)

dnl Internationalisation stuff
ALL_LINGUAS="en_AU es_ES eu_ES"

GETTEXT_PACKAGE=evosugar-eplugin-0.0
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [definition of GETTEXT_PACKAGE])
AM_GLIB_DEFINE_LOCALEDIR(GNOMELOCALEDIR)
LOCALEDIR=$localedir
AC_SUBST(LOCALEDIR)

# Generated files from automake
AC_CONFIG_FILES([
Makefile
src/Makefile
po/Makefile.in
])

AC_OUTPUT

echo ""
echo "Evosugar plugin successfully configured"
echo ""
echo "    Installation path: $PLUGIN_INSTALL_DIR"
echo ""
