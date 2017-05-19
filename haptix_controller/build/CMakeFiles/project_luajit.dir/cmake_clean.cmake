FILE(REMOVE_RECURSE
  "CMakeFiles/project_luajit"
  "CMakeFiles/project_luajit-complete"
  "luajit-2.0.1/src/project_luajit-stamp/project_luajit-install"
  "luajit-2.0.1/src/project_luajit-stamp/project_luajit-mkdir"
  "luajit-2.0.1/src/project_luajit-stamp/project_luajit-download"
  "luajit-2.0.1/src/project_luajit-stamp/project_luajit-update"
  "luajit-2.0.1/src/project_luajit-stamp/project_luajit-patch"
  "luajit-2.0.1/src/project_luajit-stamp/project_luajit-configure"
  "luajit-2.0.1/src/project_luajit-stamp/project_luajit-build"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/project_luajit.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
