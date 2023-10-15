include_directories(${CMAKE_SOURCE_DIR}/libs)
include_directories(src)
add_library(FileIOManager STATIC
    "src/Base/Compressor.cpp"
    "src/Base/Conversion.cpp"
    "src/Base/DebugStream.cpp"
    "src/Base/Halt.cpp"
    "src/Base/IBinaryStream.cpp"
    "src/Base/MemoryManager.cpp"
    "src/Base/OStringStream.cpp"
    "src/Base/PrimeNumber.cpp"
    "src/Base/RefString.cpp"
    "src/FileIO/InFile.cpp"
    "src/FileIO/Manager.cpp"
)
add_library(GraphicsManager STATIC
    "src/FileIO/OutFile.cpp"
    "src/Graphics/IndexBuffer.cpp"
    "src/Graphics/Manager.cpp"
)
add_library(InputManaegr STATIC
    "src/Graphics/Texture.cpp"
    "src/Graphics/VertexBuffer.cpp"
    "src/Input/Joystick.cpp"
    "src/Input/Keyboard.cpp"
    "src/Input/Manager.cpp"
)
add_library(MathFunctions STATIC
    "src/Input/Mouse.cpp"
    "src/Math/Functions.cpp"
)
add_library(SoundManager STATIC
    "src/Math/Matrix22.cpp"
    "src/Math/Matrix23.cpp"
    "src/Math/Matrix33.cpp"
    "src/Math/Matrix34.cpp"
    "src/Math/Matrix44.cpp"
    "src/Math/Random.cpp"
    "src/Math/vector2.cpp"
    "src/Math/Vector3.cpp"
    "src/Math/Vector4.cpp"
    "src/PseudoXml/Attribute.cpp"
    "src/PseudoXml/Document.cpp"
    "src/PseudoXml/DocumentImpl.cpp"
    "src/PseudoXml/Element.cpp"
    "src/PseudoXml/Tag.cpp"
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
    "src/Sound/Manager.cpp"
)
add_library(ThreadingFunctions STATIC
    "src/Sound/Player.cpp"
    "src/Sound/Wave.cpp"
    "src/Threading/Event.cpp"
    "src/Threading/Functions.cpp"
)

add_library(ThreadingManager STATIC
    "src/Threading/Manager.cpp"
)
