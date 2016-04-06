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

class GLBuffer {
  public:
    std::vector<int32_t> size, byteSize, glType;
    std::vector<int64_t> offset;
    std::vector<const void*> data;
    uint32_t bufferObject;
    GLBuffer();
    int32_t add(const void* buf, int32_t elemGLType, int32_t elemCount);
    void freeze();
    void update(int32_t i, std::vector<float> v);
};

class UniformValue {
  public:
    enum ::InputType::tag tag;
    int32_t* _int;
    float* _float;
};

class Stream {
  public:
    enum Type type;
    std::shared_ptr<GLBuffer> buffer;
    int32_t index;
    bool isArray;
    int32_t glSize;
    std::shared_ptr<UniformValue> attributeValue;
    Stream(float v);
    Stream(V2F v);
    Stream(V3F v);
    Stream(V4F v);
    Stream(M22F v);
    Stream(M33F v);
    Stream(M44F v);
    Stream(std::shared_ptr<GLBuffer> b, int32_t i, enum Type t);
};

class StreamMap {
  public:
    std::map<std::string,std::shared_ptr<Stream>> map;
    StreamMap();
    void add(std::string name, float v);
    void add(std::string name, V2F v);
    void add(std::string name, V3F v);
    void add(std::string name, V4F v);
    void add(std::string name, M22F v);
    void add(std::string name, M33F v);
    void add(std::string name, M44F v);
    void add(std::string name, enum Type t, std::shared_ptr<GLBuffer> b, int32_t index);
    bool validate();
};

class GLObject {
  public:
    bool enabled;
    int32_t order, glMode, glCount;
    std::map<std::string,std::shared_ptr<UniformValue>> uniforms;
    std::shared_ptr<StreamMap> streams;
    ~GLObject();
    void enable(bool visible);
    void setOrder(int32_t o);
    void setUniform(std::string name, int32_t v);
    void setUniform(std::string name, bool v);
    void setUniform(std::string name, float v);
    void setUniform(std::string name, V2I v);
    void setUniform(std::string name, V2B v);
    void setUniform(std::string name, V2F v);
    void setUniform(std::string name, V3I v);
    void setUniform(std::string name, V3B v);
    void setUniform(std::string name, V3F v);
    void setUniform(std::string name, V4I v);
    void setUniform(std::string name, V4B v);
    void setUniform(std::string name, V4F v);
    void setUniform(std::string name, M22F v);
    void setUniform(std::string name, M33F v);
    void setUniform(std::string name, M44F v);
};

class PipelineInput {
  public:
    std::map<std::string,std::vector<std::shared_ptr<GLObject>>> objectMap;
    std::map<std::string,std::shared_ptr<UniformValue>> uniforms;
    int32_t screenWidth, screenHeight;
    PipelineInput();
    std::shared_ptr<GLObject> createObject(std::string slotName, enum Primitive prim, std::shared_ptr<StreamMap> attributes, std::vector<std::string> objectUniforms);
    std::shared_ptr<GLObject> createObject(std::string slotName, enum Primitive prim, std::shared_ptr<StreamMap> attributes, std::shared_ptr<GLBuffer> indexBuffer, int32_t bufferIndex, std::vector<std::string> objectUniforms);
    void sortSlotObjects();
    void setScreenSize(int32_t w, int32_t h);
    void setUniform(std::string name, int32_t v);
    void setUniform(std::string name, bool v);
    void setUniform(std::string name, float v);
    void setUniform(std::string name, V2I v);
    void setUniform(std::string name, V2B v);
    void setUniform(std::string name, V2F v);
    void setUniform(std::string name, V3I v);
    void setUniform(std::string name, V3B v);
    void setUniform(std::string name, V3F v);
    void setUniform(std::string name, V4I v);
    void setUniform(std::string name, V4B v);
    void setUniform(std::string name, V4F v);
    void setUniform(std::string name, M22F v);
    void setUniform(std::string name, M33F v);
    void setUniform(std::string name, M44F v);
};

class Texture {
  public:
    int32_t target;
    uint32_t texture;
};

class StreamInfo {
  public:
    std::string name;
    int32_t index;
};

class GLProgram {
  public:
    uint32_t program, vertexShader, fragmentShader;
    std::map<std::string,int32_t> programUniforms, programInTextures;
    std::map<std::string,std::shared_ptr<StreamInfo>> programStreams;
    GLProgram();
};

class GLStreamData {
  public:
    int32_t glMode, glCount;
    std::shared_ptr<StreamMap> streams;
    GLStreamData();
};

class GLES20Pipeline {
  private:
    std::shared_ptr<PipelineInput> input;
    bool hasPipelineInput;
    std::shared_ptr<::data::Pipeline> pipeline;
    std::vector<std::shared_ptr<Texture>> textures;
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
