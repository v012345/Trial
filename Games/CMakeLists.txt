cmake_minimum_required(VERSION 3.20)
include_directories(include)
add_library(Base STATIC
    "src/Base/MemoryManager.cpp"
    "src/Base/Conversion.cpp"
    "src/Base/IBinaryStream.cpp"
    "src/Base/RefString.cpp"
    "src/Base/OStringStream.cpp"
    "src/Base/PrimeNumber.cpp"
    "src/Base/Compressor.cpp"
    "src/Base/DebugStream.cpp"
    "src/Base/Halt.cpp"
)
add_library(FileIOManager STATIC
    "src/FileIO/Manager.cpp"
    "src/FileIO/InFile.cpp"
    "src/FileIO/OutFile.cpp"
)
target_include_directories(FileIOManager PRIVATE src)

add_library(GraphicsManager STATIC
    "src/Graphics/IndexBuffer.cpp"
    "src/Graphics/Manager.cpp"
    "src/Graphics/Texture.cpp"
    "src/Graphics/VertexBuffer.cpp"
)
target_include_directories(GraphicsManager PRIVATE src)

add_library(ThreadingManager STATIC
    "src/Threading/Manager.cpp"
    "src/Threading/Mutex.cpp"
    "src/Threading/Event.cpp"
    "src/Threading/Functions.cpp"
    "src/Threading/Thread.cpp"
    "src/Threading/Semaphore.cpp"
)
target_include_directories(ThreadingManager PRIVATE src)

add_library(PseudoXml STATIC
    "src/PseudoXml/Attribute.cpp"
    "src/PseudoXml/Document.cpp"
    "src/PseudoXml/DocumentImpl.cpp"
    "src/PseudoXml/Element.cpp"
    "src/PseudoXml/Tag.cpp"
)
target_include_directories(PseudoXml PRIVATE src)

add_library(InputManaegr STATIC
    "src/Input/Joystick.cpp"
    "src/Input/Keyboard.cpp"
    "src/Input/Manager.cpp"
    "src/Input/Mouse.cpp"
)
target_include_directories(InputManaegr PRIVATE src)

add_library(MathFunctions STATIC
    "src/Math/Functions.cpp"
    "src/Math/Matrix22.cpp"
    "src/Math/Matrix23.cpp"
    "src/Math/Matrix33.cpp"
    "src/Math/Matrix34.cpp"
    "src/Math/Matrix44.cpp"
    "src/Math/Random.cpp"
    "src/Math/vector2.cpp"
    "src/Math/Vector3.cpp"
    "src/Math/Vector4.cpp"
)
target_include_directories(MathFunctions PRIVATE src)

add_library(SoundManager STATIC
    "src/Sound/Manager.cpp"
    "src/Sound/Player.cpp"
    "src/Sound/Wave.cpp"
)
target_include_directories(SoundManager PRIVATE src)

add_library(SceneManager STATIC
    "src/Scene/Animation.cpp"
    "src/Scene/AnimationNode.cpp"
    "src/Scene/Batch.cpp"
    "src/Scene/CollisionDetector.cpp"
    "src/Scene/CollisionMesh.cpp"
    "src/Scene/Container.cpp"
    "src/Scene/Font.cpp"
    "src/Scene/Model.cpp"
    "src/Scene/PrimitiveRenderer.cpp"
    "src/Scene/StringRenderer.cpp"
    "src/Scene/Tree.cpp"
    "src/Scene/TreeTemplate.cpp"
)
target_include_directories(SceneManager PRIVATE src)

add_library(GameLib STATIC
    "Framework.cpp"
)
target_compile_definitions(GameLib PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_SOURCE_DIR}/scripts/StartGame.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(GameLib
    Base
    PseudoXml
    FileIOManager
    ThreadingManager
    GraphicsManager
    InputManaegr
    MathFunctions
    SoundManager
    SceneManager
    winmm
    d3d9
    dsound
    dinput8
    dxguid
    liblua-5.4.6
    lfs
    png_shared
)