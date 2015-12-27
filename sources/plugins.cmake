# tab plugins
quadro_install_plugin("applauncher")
quadro_install_plugin("filemanager")
quadro_install_plugin("webpage" "webpage" ${Qt5Network_LIBRARIES})
# plugins
quadro_install_plugin("datetime")
quadro_install_plugin("desktopengine")
quadro_install_plugin("helloworld")
quadro_install_plugin("yahooweather" "yahooweather" ${Qt5Network_LIBRARIES})
quadro_install_plugin("newsplugin" "newsplugin" ${Qt5Network_LIBRARIES})
quadro_install_plugin("pcstatus" "pcstatus" ${Qt5Network_LIBRARIES})
