#ifndef HEADER_LambdaCube_H
#define HEADER_LambdaCube_H

#include "IR.hpp"

enum class Primitive {
  TriangleStrip,
  TriangleList,
  TriangleFan,
  LineStrip,
  LineList,
  LineLoop,
  PointList
};

enum class Type {
  FLOAT,
  FLOAT_VEC2,
  FLOAT_VEC3,
  FLOAT_VEC4,
  FLOAT_MAT2,
  FLOAT_MAT3,
  FLOAT_MAT4
};

class Buffer {
  public:
    std::vector<int32_t> size, byteSize, glType;
    std::vector<int64_t> offset;
    std::vector<void*> data;
    uint32_t bufferObject;
    int32_t add(std::vector<int8_t>& v);
    int32_t add(std::vector<uint8_t>& v);
    int32_t add(std::vector<int16_t>& v);
    int32_t add(std::vector<uint16_t>& v);
    int32_t add(std::vector<float>& v);
    void freeze();
    void update(int32_t i, std::vector<float>& v);
};

class Stream {
  public:
    Type type;
    std::shared_ptr<Buffer> buffer;
    int32_t index;
    bool isArray;
    int32_t glSize;
    union {
      float _float;
      V2F _v2f;
      V3F _v3f;
      V4F _v4f;
      M22F _m22f;
      M33F _m33f;
      M44F _m44f;
    };
    Stream(float& v);
    Stream(V2F& v);
    Stream(V3F& v);
    Stream(V4F& v);
    Stream(M22F& v);
    Stream(M33F& v);
    Stream(M44F& v);
    Stream(std::shared_ptr<Buffer> b, int32_t i, Type t);
};

class StreamMap {
  public:
    std::map<std::string,std::shared_ptr<Stream>> map;
    void add(std::string name, float& v);
    void add(std::string name, V2F& v);
    void add(std::string name, V3F& v);
    void add(std::string name, V4F& v);
    void add(std::string name, M22F& v);
    void add(std::string name, M33F& v);
    void add(std::string name, M44F& v);
    void add(std::string name, Type t, std::shared_ptr<Buffer> b, int32_t index);
    bool validate();
};

struct UniformValue {
  enum InputType::tag tag;
  union {
    int32_t _int;
    uint32_t _word;
    float _float;
    bool _bool;
    V2I _v2i;
    V2U _v2u;
    V2B _v2b;
    V2F _v2f;
    V3I _v3i;
    V3U _v3u;
    V3B _v3b;
    V3F _v3f;
    V4I _v4i;
    V4U _v4u;
    V4B _v4b;
    V4F _v4f;
    M22F _m22f;
    M33F _m33f;
    M44F _m44f;
  };
};

class Object {
  public:
    bool enabled;
    int32_t order, glMode, glCount;
    std::map<std::string,UniformValue> uniforms;
    std::shared_ptr<StreamMap> streams;
    ~Object();
    void enable(bool visible);
    void setOrder(int32_t o);
    void setUniform(std::string name, int32_t& v);
    void setUniform(std::string name, uint32_t& v);
    void setUniform(std::string name, float& v);
    void setUniform(std::string name, bool& v);
    void setUniform(std::string name, V2I& v);
    void setUniform(std::string name, V2U& v);
    void setUniform(std::string name, V2F& v);
    void setUniform(std::string name, V2B& v);
    void setUniform(std::string name, V3I& v);
    void setUniform(std::string name, V3U& v);
    void setUniform(std::string name, V3F& v);
    void setUniform(std::string name, V3B& v);
    void setUniform(std::string name, V4I& v);
    void setUniform(std::string name, V4U& v);
    void setUniform(std::string name, V4F& v);
    void setUniform(std::string name, V4B& v);
    void setUniform(std::string name, M22F& v);
    void setUniform(std::string name, M33F& v);
    void setUniform(std::string name, M44F& v);
};

class PipelineInput {
  public:
    std::map<std::string,std::shared_ptr<std::vector<std::shared_ptr<Object>>>> objectMap;
    std::map<std::string,UniformValue> uniforms;
    int32_t screenWidth, screenHeight;
    std::shared_ptr<Object> createObject(std::string slotName, Primitive prim, std::shared_ptr<StreamMap> attributes, std::vector<std::string> objectUniforms);
    std::shared_ptr<Object> createObject(std::string slotName, Primitive prim, StreamMap& attributes, Buffer& indexBuffer, int32_t bufferIndex, std::vector<std::string> objectUniforms);
    void sortSlotObjects();
    void setScreenSize(int32_t w, int32_t h);
    void setUniform(std::string name, int32_t& v);
    void setUniform(std::string name, uint32_t& v);
    void setUniform(std::string name, float& v);
    void setUniform(std::string name, bool& v);
    void setUniform(std::string name, V2I& v);
    void setUniform(std::string name, V2U& v);
    void setUniform(std::string name, V2F& v);
    void setUniform(std::string name, V2B& v);
    void setUniform(std::string name, V3I& v);
    void setUniform(std::string name, V3U& v);
    void setUniform(std::string name, V3F& v);
    void setUniform(std::string name, V3B& v);
    void setUniform(std::string name, V4I& v);
    void setUniform(std::string name, V4U& v);
    void setUniform(std::string name, V4F& v);
    void setUniform(std::string name, V4B& v);
    void setUniform(std::string name, M22F& v);
    void setUniform(std::string name, M33F& v);
    void setUniform(std::string name, M44F& v);
};

struct Texture {
  int32_t target;
  uint32_t texture;
};

struct StreamInfo {
  std::string name;
  int32_t index;
};

class GLProgram {
  public:
    uint32_t program, vertexShader, fragmentShader;
    std::map<std::string,int32_t> programUniforms, programInTextures;
    std::map<std::string,StreamInfo> programStreams;
};

struct GLStreamData {
  int32_t glMode, glCount;
  StreamMap streams;
};

class GLES20Pipeline {
  private:
    std::shared_ptr<PipelineInput> input;
    std::shared_ptr<data::Pipeline> pipeline;
    std::vector<Texture> textures;
    std::vector<uint32_t> targets;
    std::vector<std::shared_ptr<GLProgram>> programs;
    std::vector<std::shared_ptr<GLStreamData>> streamData;
    uint32_t currentProgram;
    bool hasCurrentProgram;
    uint32_t createRenderTarget(std::shared_ptr<RenderTarget> t_);
  public:
    uint32_t screenTarget;
    GLES20Pipeline(std::shared_ptr<Pipeline> ppl_);
    ~GLES20Pipeline();
    void setPipelineInput(std::shared_ptr<PipelineInput> i);
    void render();
};

#endif
