
INCLUDES =						\
	-I/usr/include/gsoap/				\
	-I$(EVOLUTION_SOURCE)				\
	-I$(top_srcdir)					\
	-DGETTEXT_PACKAGE="\"$(GETTEXT_PACKAGE)\""	\
	-DWITH_OPENSSL					\
	-DLOCALEDIR="\"$(LOCALEDIR)\""			\
	-DPLUGIN_INSTALL_DIR="\"$(PLUGIN_INSTALL_DIR)\""        \
	$(TEST_EPLUGIN_CFLAGS)

%.eplug.in: %.eplug.xml
	LC_ALL=C $(INTLTOOL_MERGE) -x -u /tmp $< $@

%.eplug: %.eplug.in
	sed -e "s%+PLUGIN_INSTALL_DIR+%$(PLUGIN_INSTALL_DIR)%" \
	-e "s%+GETTEXT_PACKAGE+%$(GETTEXT_PACKAGE)%" \
	-e "s%+LOCALEDIR+%$(LOCALEDIR)%" \
	$< > $@

%.error.in: %.error.xml
	LC_ALL=C $(INTLTOOL_MERGE) -x -u /tmp $< $@

%.error: %.error.in
	sed -e "s%+PLUGIN_INSTALL_DIR+%$(PLUGIN_INSTALL_DIR)%" \
	-e "s%+GETTEXT_PACKAGE+%$(GETTEXT_PACKAGE)%" \
	-e "s%+LOCALEDIR+%$(LOCALEDIR)%" \
	$< > $@

errordir = $(ERROR_DIR)
error_DATA = com-irontec-evosugar.error

plugindir = $(PLUGIN_INSTALL_DIR)
plugin_DATA = com-irontec-evosugar.eplug gui/evosugar.ui #TODO 
plugin_LTLIBRARIES = libcom-irontec-evosugar.la

libcom_irontec_evosugar_la_SOURCES = evosugar.c scrm.c lib/soapC.c lib/soapClient.c
libcom_irontec_evosugar_la_LDFLAGS = -module -avoid-version -Wl,--wrap -Wl,soap_new -Wl,--wrap -Wl,soap_init

schemadir       = $(GCONF_SCHEMA_FILE_DIR)
schema_in_files = apps-evolution-evosugar.schemas.in
schema_DATA     = $(schema_in_files:.schemas.in=.schemas)

@INTLTOOL_SCHEMAS_RULE@

install-data-local:
        if test -z "$(DESTDIR)" ; then                                                  \
                for p in $(schema_DATA) ; do                                            \
                        GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE)               \
                                $(GCONFTOOL) --makefile-install-rule $$p;               \
                done                                                                    \
        fi



EXTRA_DIST = com-irontec-evosugar.eplug.xml \
	com-irontec-evosugar.error.xml

CLEANFILES = com-irontec-evosugar.error com-irontec-evosugar.eplug


