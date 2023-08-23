cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

PROJECT(SqDrv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(VTK 9 COMPONENTS
  CommonColor
  CommonCore
  DomainsChemistry
  FiltersSources
  IOChemistry
  IOGeometry
  IOImage
  ImagingCore
  ImagingSources
  InteractionStyle
  RenderingCore
  RenderingLabel
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingLOD
  RenderingOpenGL2
  GUISupportQt
  REQUIRED QUIET)
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

find_package(Qt6 COMPONENTS REQUIRED Core Widgets Gui PrintSupport OpenGL)
if (NOT Qt6_FOUND)
    #find_package(Qt5 5.15 REQUIRED COMPONENTS Core)
find_package(Qt5 5.15 COMPONENTS REQUIRED Core Widgets Gui PrintSupport OpenGL)
endif()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB UI_FILES *.ui)
file(GLOB HDR_FILES *.h)
file(GLOB CXX_FILES *.cxx)
file(GLOB CPP_FILES *.cpp)
file(GLOB RC_FILES *.qrc)

qt_wrap_ui(UISources ${UI_FILES} )

if(Windows)
  target_link_libraries(SqDrv Qt::WinMain)
endif()

# CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
add_executable(SqDrv MACOSX_BUNDLE ${CXX_FILES} ${CPP_FILES} ${UISources} ${RC_FILES} ${HDR_FILES})

target_link_libraries(SqDrv ${VTK_LIBRARIES} Qt::Core Qt::Gui Qt::Widgets Qt::PrintSupport Qt::OpenGL)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SqDrv)
endif()

