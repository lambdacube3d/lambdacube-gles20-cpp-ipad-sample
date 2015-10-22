#ifndef HEADER_LambdaCube_H
#define HEADER_LambdaCube_H

#include "IR.hpp"

// question: allow client memory or only gpu buffer?
// answer: try both
class Buffer {
  public:
    int add(std::vector<char> &v);
    int add(std::vector<unsigned char> &v);
    int add(std::vector<short> &v);
    int add(std::vector<unsigned short> &v);
    int add(std::vector<float> &v);
    void freeze();
    void update(int i, std::vector<float> &v);
    // TODO

  //private:
    std::vector<int> size;
    std::vector<int> byteSize;
    std::vector<long> offset;
    std::vector<void*> data;
    std::vector<int> glType;
    unsigned int bufferObject;
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

class Stream {
  public:
    Stream(Float& v);
    Stream(V2F& v);
    Stream(V3F& v);
    Stream(V4F& v);
    Stream(M22F& v);
    Stream(M33F& v);
    Stream(M44F& v);
    Stream(std::shared_ptr<Buffer> b, int index, Type t);

  //private:
    Type type;
    std::shared_ptr<Buffer> buffer;
    int index;
    bool isArray;
    int glSize;
    union {
      Float _float;
      V2F   _v2f;
      V3F   _v3f;
      V4F   _v4f;
      M22F  _m22f;
      M33F  _m33f;
      M44F  _m44f;
    };
};

class StreamMap {
  public:
    void add(std::string name, Float& v);
    void add(std::string name, V2F& v);
    void add(std::string name, V3F& v);
    void add(std::string name, V4F& v);
    void add(std::string name, M22F& v);
    void add(std::string name, M33F& v);
    void add(std::string name, M44F& v);
    void add(std::string name, Type t, std::shared_ptr<Buffer> b, int index);
    bool validate();

  //private:
    std::map<std::string,std::shared_ptr<Stream>> map;
};

enum class Primitive {
  TriangleStrip,
  TriangleList,
  TriangleFan,
  LineStrip,
  LineList,
  LineLoop,
  PointList
};

typedef struct {
  enum InputType::tag tag;
  union {
    Int   _int;
    Word  _word;
    Float _float;
    Bool  _bool;
    V2I   _v2i;
    V2U   _v2u;
    V2B   _v2b;
    V2F   _v2f;
    V3I   _v3i;
    V3U   _v3u;
    V3B   _v3b;
    V3F   _v3f;
    V4I   _v4i;
    V4U   _v4u;
    V4B   _v4b;
    V4F   _v4f;
    M22F  _m22f;
    M33F  _m33f;
    M44F  _m44f;
  };
} UniformValue;

class Object {
  public:
    ~Object();
    void enable(bool visible);
    void setOrder(int order);

    void setUniform(std::string name, Int& v);
    void setUniform(std::string name, Word& v);
    void setUniform(std::string name, Float& v);
    void setUniform(std::string name, Bool& v);
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

  //private:
    bool enabled;
    int order;
    int glMode;
    int glCount;
    std::map<std::string,UniformValue> uniforms;
    std::shared_ptr<StreamMap> streams;
};

class PipelineInput {
  public:
    std::shared_ptr<Object> createObject(String slotName, Primitive prim, std::shared_ptr<StreamMap> attributes, std::vector<String> objectUniforms);
    std::shared_ptr<Object> createObject(String slotName, Primitive prim, StreamMap& attributes, Buffer& indexBuffer, int bufferIndex, std::vector<String> objectUniforms);
    void sortSlotObjects();
    void setScreenSize(int w,int h);

    void setUniform(std::string name, Int& v);
    void setUniform(std::string name, Word& v);
    void setUniform(std::string name, Float& v);
    void setUniform(std::string name, Bool& v);
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

  //private:
    std::map<std::string,std::shared_ptr<std::vector<std::shared_ptr<Object>>>> objectMap;
    std::map<std::string,UniformValue> uniforms;
    int screenWidth;
    int screenHeight;
};

struct Texture {
  int           target;
  unsigned int  texture;
};

struct StreamInfo {
  std::string name;
  int         index;
};

class GLProgram {
  public:
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    std::map<::UniformName, int> programUniforms;
    std::map<::UniformName, int> programInTextures;
    std::map<::UniformName, StreamInfo> programStreams;
};

struct GLStreamData {
  int glMode;
  int glCount;
  StreamMap streams;
};

class GLES20Pipeline {
  public:
    GLES20Pipeline(std::shared_ptr<Pipeline> ppl);
    ~GLES20Pipeline();

    void setPipelineInput(std::shared_ptr<PipelineInput>);
    void render();

    unsigned int screenTarget;

  private:
    unsigned int createRenderTarget(std::shared_ptr<RenderTarget> t_);

    std::shared_ptr<PipelineInput> input;
    std::shared_ptr<data::Pipeline> pipeline;
    std::vector<Texture> textures;
    std::vector<unsigned int> targets;
    std::vector<std::shared_ptr<GLProgram>> programs;
    std::vector<std::shared_ptr<GLStreamData>> streamData;
    unsigned int currentProgram;
    bool hasCurrentProgram;
};

#endif