#include <iostream>
#include <OpenGLES/ES2/gl.h>

#include "LambdaCube.hpp"

/*
    // input setup
    PipelineInput input;
    Buffer b;
    StreamMap attributes;
    attributes.add("position", FLOAT_VEC3, b, b.add({1.0,2.0,3.0}));
    attributes.add("normal",   FLOAT_VEC3, b, b.add({1.0,2.0,3.0}));
    b.freeze();
    attributes.validate();

    // add game 3D object
    auto obj = input.createObject("slotName", TriangleList, attributes, {"ModelMatrix"});
    M44F mat;
    obj.setUniform("ModelMatrix", mat);

    // render loop
    pipeline.render();
*/

/*
  pipeline input:
    uniform types:
      FLOAT FLOAT_VEC2 FLOAT_VEC3 FLOAT_VEC4
      INT INT_VEC2 INT_VEC3 INT_VEC4
      BOOL BOOL_VEC2 BOOL_VEC3 BOOL_VEC4
      FLOAT_MAT2 FLOAT_MAT3 FLOAT_MAT4
      SAMPLER_2D SAMPLER_CUBE
    void Uniform{1234}{if}( int location, T value );
    void Uniform{1234}{if}v( int location, sizei count, T value );
    void UniformMatrix{234}fv( int location, sizei count, booleantranspose,const float*value);

    attribute:
      possible types:
        FLOAT
        FLOAT_VEC2
        FLOAT_VEC3
        FLOAT_VEC4
        FLOAT_MAT2
        FLOAT_MAT3
        FLOAT_MAT4
    void VertexAttrib{1234}{f}( uint index, T values );
    void VertexAttrib{1234}{f}v( uint index, T values );
    void VertexAttribPointer( uint index, int size, enum type, boolean normalized, sizei stride, const void *pointer );
      size:
        1,2,3,4
      type:
        BYTE
        UNSIGNED_BYTE
        SHORT
        UNSIGNED_SHORT
        FIXED
        FLOAT
    void EnableVertexAttribArray( uint index );
    void DisableVertexAttribArray( uint index );

    draw:
    void DrawArrays( enum mode, int first, sizei count );
    void DrawElements( enum mode, sizei count, enum type, void *indices );
*/

enum Type inputType(std::shared_ptr<InputType> t) {
  switch (t->tag) {
    case InputType::tag::Float: return Type::FLOAT;
    case InputType::tag::V2F:   return Type::FLOAT_VEC2;
    case InputType::tag::V3F:   return Type::FLOAT_VEC3;
    case InputType::tag::V4F:   return Type::FLOAT_VEC4;
    case InputType::tag::M22F:  return Type::FLOAT_MAT2;
    case InputType::tag::M33F:  return Type::FLOAT_MAT3;
    case InputType::tag::M44F:  return Type::FLOAT_MAT4;
  }
  throw "illegal input type";
}

int primitiveMode(Primitive p) {
  switch (p) {
    case Primitive::TriangleStrip:  return GL_TRIANGLE_STRIP;
    case Primitive::TriangleList:   return GL_TRIANGLES;
    case Primitive::TriangleFan:    return GL_TRIANGLE_FAN;
    case Primitive::LineStrip:      return GL_LINE_STRIP;
    case Primitive::LineList:       return GL_LINES;
    case Primitive::LineLoop:       return GL_LINE_LOOP;
    case Primitive::PointList:      return GL_POINTS;
  }
}

void setUniformValue(int i, UniformValue& v) {
  switch (v.tag) {
    case InputType::tag::Int:   glUniform1i(i,v._int);              break;
    case InputType::tag::Word:  glUniform1i(i,v._word);             break;
    case InputType::tag::Float: glUniform1f(i,v._float);            break;
    case InputType::tag::Bool:  glUniform1i(i,v._bool);             break;
    case InputType::tag::V2I:   glUniform2iv(i,1,(int*)&v._v2i);    break;
    case InputType::tag::V2U:   glUniform2iv(i,1,(int*)&v._v2u);    break;
    case InputType::tag::V2F:   glUniform2fv(i,1,(float*)&v._v2f);  break;
    case InputType::tag::V2B:   glUniform2iv(i,1,(int*)&v._v2b);    break;
    case InputType::tag::V3I:   glUniform3iv(i,1,(int*)&v._v3i);    break;
    case InputType::tag::V3U:   glUniform3iv(i,1,(int*)&v._v3u);    break;
    case InputType::tag::V3F:   glUniform3fv(i,1,(float*)&v._v3f);  break;
    case InputType::tag::V3B:   glUniform3iv(i,1,(int*)&v._v3b);    break;
    case InputType::tag::V4I:   glUniform4iv(i,1,(int*)&v._v4i);    break;
    case InputType::tag::V4U:   glUniform4iv(i,1,(int*)&v._v4u);    break;
    case InputType::tag::V4F:   glUniform4fv(i,1,(float*)&v._v4f);  break;
    case InputType::tag::V4B:   glUniform4iv(i,1,(int*)&v._v4b);    break;
    case InputType::tag::M22F:  glUniformMatrix2fv(i,1,false,(float*)&v._m22f); break;
    case InputType::tag::M33F:  glUniformMatrix3fv(i,1,false,(float*)&v._m33f); break;
    case InputType::tag::M44F:  glUniformMatrix4fv(i,1,false,(float*)&v._m44f); break;
    default:;
  }
}

void setStream(int i, Stream& s) {
  if (s.isArray) {
    glBindBuffer(GL_ARRAY_BUFFER,s.buffer->bufferObject);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, s.glSize, s.buffer->glType[s.index], false, 0, (const void*)s.buffer->offset[s.index]);
  } else {
    glDisableVertexAttribArray(i);
    switch (s.type) {
      case Type::FLOAT:       glVertexAttrib1f(i,s._float);               break;
      case Type::FLOAT_VEC2:  glVertexAttrib2fv(i,  (float*)&s._v2f);     break;
      case Type::FLOAT_VEC3:  glVertexAttrib3fv(i,  (float*)&s._v3f);     break;
      case Type::FLOAT_VEC4:  glVertexAttrib4fv(i,  (float*)&s._v4f);     break;
      case Type::FLOAT_MAT2:  glVertexAttrib2fv(i,  (float*)&s._m22f.x);
                              glVertexAttrib2fv(i+1,(float*)&s._m22f.y);  break;
      case Type::FLOAT_MAT3:  glVertexAttrib3fv(i,  (float*)&s._m33f.x);
                              glVertexAttrib3fv(i+1,(float*)&s._m33f.y);
                              glVertexAttrib3fv(i+2,(float*)&s._m33f.z);  break;
      case Type::FLOAT_MAT4:  glVertexAttrib4fv(i,  (float*)&s._m44f.x);
                              glVertexAttrib4fv(i+1,(float*)&s._m44f.y);
                              glVertexAttrib4fv(i+2,(float*)&s._m44f.z);
                              glVertexAttrib4fv(i+3,(float*)&s._m44f.w);  break;
    }
  }
}

// pipeline input
void Buffer::update(int i, std::vector<float> &v) {
}

int Buffer::add(std::vector<char> &v) {
  int i = data.size();
  data.push_back(v.data());
  size.push_back(v.size());
  byteSize.push_back(sizeof(char)*v.size());
  glType.push_back(GL_BYTE);
  return i;
}

int Buffer::add(std::vector<unsigned char> &v) {
  int i = data.size();
  data.push_back(v.data());
  size.push_back(v.size());
  byteSize.push_back(sizeof(unsigned char)*v.size());
  glType.push_back(GL_UNSIGNED_BYTE);
  return i;
}

int Buffer::add(std::vector<short> &v) {
  int i = data.size();
  data.push_back(v.data());
  size.push_back(v.size());
  byteSize.push_back(sizeof(short)*v.size());
  glType.push_back(GL_SHORT);
  return i;
}

int Buffer::add(std::vector<unsigned short> &v) {
  int i = data.size();
  data.push_back(v.data());
  size.push_back(v.size());
  byteSize.push_back(sizeof(unsigned short)*v.size());
  glType.push_back(GL_UNSIGNED_SHORT);
  return i;
}

int Buffer::add(std::vector<float> &v) {
  int i = data.size();
  data.push_back(v.data());
  size.push_back(v.size());
  byteSize.push_back(sizeof(float)*v.size());
  glType.push_back(GL_FLOAT);
  return i;
}

/*
  void BindBuffer( enum target, uint buffer );
    target:
      ARRAY_BUFFER
  void GenBuffers( sizei n, uint *buffers );
  void DeleteBuffers( sizei n, const uint *buffers );
  void BufferData( enum target, sizeiptr size, const void *data, enum usage );
    usage:
      STATIC_DRAW
      DYNAMIC_DRAW
      STREAM_DRAW
  void BufferSubData( enum target, intptr offset, sizeiptr size,const void*data);
*/
void Buffer::freeze() {
  unsigned int bufferSize = 0;
  for (auto i : byteSize) {
    offset.push_back(bufferSize);
    bufferSize += i;
  }
  unsigned int bo;
  glGenBuffers(1,&bo);
  glBindBuffer(GL_ARRAY_BUFFER,bo);
  glBufferData(GL_ARRAY_BUFFER,bufferSize,nullptr,GL_STATIC_DRAW);
  unsigned int offset_ = 0;
  for (int i = 0; i < data.size(); i++) {
    glBufferSubData(GL_ARRAY_BUFFER, offset_, byteSize[i], data[i]);
    offset_ += byteSize[i];
  }
  bufferObject = bo;
}

// Stream
Stream::Stream(Float& v) {
  type = Type::FLOAT;
  isArray = false;
  _float = v;
  glSize = 1;
}
Stream::Stream(V2F& v) {
  type = Type::FLOAT_VEC2;
  isArray = false;
  _v2f = v;
  glSize = 2;
}
Stream::Stream(V3F& v) {
  type = Type::FLOAT_VEC3;
  isArray = false;
  _v3f = v;
  glSize = 3;
}
Stream::Stream(V4F& v) {
  type = Type::FLOAT_VEC4;
  isArray = false;
  _v4f = v;
  glSize = 4;
}
Stream::Stream(M22F& v) {
  type = Type::FLOAT_MAT2;
  isArray = false;
  _m22f = v;
  glSize = 4;
}
Stream::Stream(M33F& v) {
  type = Type::FLOAT_MAT3;
  isArray = false;
  _m33f = v;
  glSize = 9;
}
Stream::Stream(M44F& v) {
  type = Type::FLOAT_MAT4;
  isArray = false;
  _m44f = v;
  glSize = 16;
}
Stream::Stream(std::shared_ptr<Buffer> b, int i, Type t) {
  type = t;
  buffer = b;
  index = i;
  isArray = true;
  switch (t) {
    case Type::FLOAT:       glSize = 1; break;
    case Type::FLOAT_VEC2:  glSize = 2; break;
    case Type::FLOAT_VEC3:  glSize = 3; break;
    case Type::FLOAT_VEC4:  glSize = 4; break;
    case Type::FLOAT_MAT2:  glSize = 2; break;
    case Type::FLOAT_MAT3:  glSize = 9; break;
    case Type::FLOAT_MAT4:  glSize = 16;break;
  }
}

// StreamMap
void StreamMap::add(std::string name, Float& v) {
  map[name] = std::shared_ptr<Stream>(new Stream(v));
}
void StreamMap::add(std::string name, V2F& v) {
  map[name] = std::shared_ptr<Stream>(new Stream(v));
}
void StreamMap::add(std::string name, V3F& v) {
  map[name] = std::shared_ptr<Stream>(new Stream(v));
}
void StreamMap::add(std::string name, V4F& v) {
  map[name] = std::shared_ptr<Stream>(new Stream(v));
}
void StreamMap::add(std::string name, M22F& v) {
  map[name] = std::shared_ptr<Stream>(new Stream(v));
}
void StreamMap::add(std::string name, M33F& v) {
  map[name] = std::shared_ptr<Stream>(new Stream(v));
}
void StreamMap::add(std::string name, M44F& v) {
  map[name] = std::shared_ptr<Stream>(new Stream(v));
}
void StreamMap::add(std::string name, Type t, std::shared_ptr<Buffer> b, int index) {
  map[name] = std::shared_ptr<Stream>(new Stream(b,index,t));
}
bool StreamMap::validate() {
  return true;
}

// Object
Object::~Object() {
}
void Object::enable(bool visible) {
  enabled = visible;
}
void Object::setOrder(int o) {
  order = o;
}
void Object::setUniform(std::string name, Int& v) {
  uniforms[name] = {.tag = InputType::tag::Int, ._int = v};
}
void Object::setUniform(std::string name, Word& v) {
  uniforms[name] = {.tag = InputType::tag::Word, ._word = v};
}
void Object::setUniform(std::string name, Float& v) {
  uniforms[name] = {.tag = InputType::tag::Float, ._float = v};
}
void Object::setUniform(std::string name, Bool& v) {
  uniforms[name] = {.tag = InputType::tag::Bool, ._bool = v};
}
void Object::setUniform(std::string name, V2I& v) {
  uniforms[name] = {.tag = InputType::tag::V2I, ._v2i = v};
}
void Object::setUniform(std::string name, V2U& v) {
  uniforms[name] = {.tag = InputType::tag::V2U, ._v2u = v};
}
void Object::setUniform(std::string name, V2F& v) {
  uniforms[name] = {.tag = InputType::tag::V2F, ._v2f = v};
}
void Object::setUniform(std::string name, V2B& v) {
  uniforms[name] = {.tag = InputType::tag::V2B, ._v2b = v};
}
void Object::setUniform(std::string name, V3I& v) {
  uniforms[name] = {.tag = InputType::tag::V3I, ._v3i = v};
}
void Object::setUniform(std::string name, V3U& v) {
  uniforms[name] = {.tag = InputType::tag::V3U, ._v3u = v};
}
void Object::setUniform(std::string name, V3F& v) {
  uniforms[name] = {.tag = InputType::tag::V3F, ._v3f = v};
}
void Object::setUniform(std::string name, V3B& v) {
  uniforms[name] = {.tag = InputType::tag::V3B, ._v3b = v};
}
void Object::setUniform(std::string name, V4I& v) {
  uniforms[name] = {.tag = InputType::tag::V4I, ._v4i = v};
}
void Object::setUniform(std::string name, V4U& v) {
  uniforms[name] = {.tag = InputType::tag::V4U, ._v4u = v};
}
void Object::setUniform(std::string name, V4F& v) {
  uniforms[name] = {.tag = InputType::tag::V4F, ._v4f = v};
}
void Object::setUniform(std::string name, V4B& v) {
  uniforms[name] = {.tag = InputType::tag::V4B, ._v4b = v};
}
void Object::setUniform(std::string name, M22F& v) {
  uniforms[name] = {.tag = InputType::tag::M22F, ._m22f = v};
}
void Object::setUniform(std::string name, M33F& v) {
  uniforms[name] = {.tag = InputType::tag::M33F, ._m33f = v};
}
void Object::setUniform(std::string name, M44F& v) {
  uniforms[name] = {.tag = InputType::tag::M44F, ._m44f = v};
}

// PipelineInput
std::shared_ptr<Object> PipelineInput::createObject(String slotName, Primitive prim, std::shared_ptr<StreamMap> attributes, std::vector<String> objectUniforms) {
  std::shared_ptr<Object> o(new Object());
  o->enabled = true;
  o->order = 0;
  o->glMode = primitiveMode(prim);
  int count = 0;
  for (auto i : attributes->map) {
    if (i.second->isArray) {
      count = i.second->buffer->size[i.second->index] / i.second->glSize;
      break;
    }
  }
  o->glCount = count;
  o->streams = attributes;
  if (objectMap.count(slotName)>0) {
    objectMap[slotName]->push_back(o);
  } else {
    objectMap[slotName] = std::shared_ptr<std::vector<std::shared_ptr<Object>>>(new std::vector<std::shared_ptr<Object>>({o}));
  }
  return o;
}
std::shared_ptr<Object> PipelineInput::createObject(String slotName, Primitive prim, StreamMap& attributes, Buffer& indexBuffer, int bufferIndex, std::vector<String> objectUniforms) {
  // TODO
  return std::shared_ptr<Object>();
}
void PipelineInput::sortSlotObjects() {
}
void PipelineInput::setScreenSize(int w,int h) {
  screenWidth = w;
  screenHeight = h;
}

void PipelineInput::setUniform(std::string name, Int& v) {
  uniforms[name] = {.tag = InputType::tag::Int, ._int = v};
}
void PipelineInput::setUniform(std::string name, Word& v) {
  uniforms[name] = {.tag = InputType::tag::Word, ._word = v};
}
void PipelineInput::setUniform(std::string name, Float& v) {
  uniforms[name] = {.tag = InputType::tag::Float, ._float = v};
}
void PipelineInput::setUniform(std::string name, Bool& v) {
  uniforms[name] = {.tag = InputType::tag::Bool, ._bool = v};
}
void PipelineInput::setUniform(std::string name, V2I& v) {
  uniforms[name] = {.tag = InputType::tag::V2I, ._v2i = v};
}
void PipelineInput::setUniform(std::string name, V2U& v) {
  uniforms[name] = {.tag = InputType::tag::V2U, ._v2u = v};
}
void PipelineInput::setUniform(std::string name, V2F& v) {
  uniforms[name] = {.tag = InputType::tag::V2F, ._v2f = v};
}
void PipelineInput::setUniform(std::string name, V2B& v) {
  uniforms[name] = {.tag = InputType::tag::V2B, ._v2b = v};
}
void PipelineInput::setUniform(std::string name, V3I& v) {
  uniforms[name] = {.tag = InputType::tag::V3I, ._v3i = v};
}
void PipelineInput::setUniform(std::string name, V3U& v) {
  uniforms[name] = {.tag = InputType::tag::V3U, ._v3u = v};
}
void PipelineInput::setUniform(std::string name, V3F& v) {
  uniforms[name] = {.tag = InputType::tag::V3F, ._v3f = v};
}
void PipelineInput::setUniform(std::string name, V3B& v) {
  uniforms[name] = {.tag = InputType::tag::V3B, ._v3b = v};
}
void PipelineInput::setUniform(std::string name, V4I& v) {
  uniforms[name] = {.tag = InputType::tag::V4I, ._v4i = v};
}
void PipelineInput::setUniform(std::string name, V4U& v) {
  uniforms[name] = {.tag = InputType::tag::V4U, ._v4u = v};
}
void PipelineInput::setUniform(std::string name, V4F& v) {
  uniforms[name] = {.tag = InputType::tag::V4F, ._v4f = v};
}
void PipelineInput::setUniform(std::string name, V4B& v) {
  uniforms[name] = {.tag = InputType::tag::V4B, ._v4b = v};
}
void PipelineInput::setUniform(std::string name, M22F& v) {
  uniforms[name] = {.tag = InputType::tag::M22F, ._m22f = v};
}
void PipelineInput::setUniform(std::string name, M33F& v) {
  uniforms[name] = {.tag = InputType::tag::M33F, ._m33f = v};
}
void PipelineInput::setUniform(std::string name, M44F& v) {
  uniforms[name] = {.tag = InputType::tag::M44F, ._m44f = v};
}

// Pipeline
int blendingFactor(std::shared_ptr<::BlendingFactor> bf) {
  switch (bf->tag) {
    case BlendingFactor::tag::ConstantAlpha:          return GL_CONSTANT_ALPHA;
    case BlendingFactor::tag::ConstantColor:          return GL_CONSTANT_COLOR;
    case BlendingFactor::tag::DstAlpha:               return GL_DST_ALPHA;
    case BlendingFactor::tag::DstColor:               return GL_DST_COLOR;
    case BlendingFactor::tag::One:                    return GL_ONE;
    case BlendingFactor::tag::OneMinusConstantAlpha:  return GL_ONE_MINUS_CONSTANT_ALPHA;
    case BlendingFactor::tag::OneMinusConstantColor:  return GL_ONE_MINUS_CONSTANT_COLOR;
    case BlendingFactor::tag::OneMinusDstAlpha:       return GL_ONE_MINUS_DST_ALPHA;
    case BlendingFactor::tag::OneMinusDstColor:       return GL_ONE_MINUS_DST_COLOR;
    case BlendingFactor::tag::OneMinusSrcAlpha:       return GL_ONE_MINUS_SRC_ALPHA;
    case BlendingFactor::tag::OneMinusSrcColor:       return GL_ONE_MINUS_SRC_COLOR;
    case BlendingFactor::tag::SrcAlpha:               return GL_SRC_ALPHA;
    case BlendingFactor::tag::SrcAlphaSaturate:       return GL_SRC_ALPHA_SATURATE;
    case BlendingFactor::tag::SrcColor:               return GL_SRC_COLOR;
    case BlendingFactor::tag::Zero:                   return GL_ZERO;
  }
  throw "illegal blending factor";
}

int blendEquation(std::shared_ptr<::BlendEquation> be) {
  switch (be->tag) {
    case BlendEquation::tag::FuncAdd:             return GL_FUNC_ADD;
    case BlendEquation::tag::FuncReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
    case BlendEquation::tag::FuncSubtract:        return GL_FUNC_SUBTRACT;
  }
  throw "illegal blend equation";
}

int comparisonFunction(std::shared_ptr<::ComparisonFunction> cf) {
  switch (cf->tag) {
    case ComparisonFunction::tag::Always:   return GL_ALWAYS;
    case ComparisonFunction::tag::Equal:    return GL_EQUAL;
    case ComparisonFunction::tag::Gequal:   return GL_GEQUAL;
    case ComparisonFunction::tag::Greater:  return GL_GREATER;
    case ComparisonFunction::tag::Lequal:   return GL_LEQUAL;
    case ComparisonFunction::tag::Less:     return GL_LESS;
    case ComparisonFunction::tag::Never:    return GL_NEVER;
    case ComparisonFunction::tag::Notequal: return GL_NOTEQUAL;
  }
  throw "illegal comparison function";
}

int frontFace(std::shared_ptr<::FrontFace> ff) {
  switch (ff->tag) {
    case FrontFace::tag::CCW: return GL_CCW;
    case FrontFace::tag::CW:  return GL_CW;
  }
  throw "illegal front face value";
}

/*
      ALPHA
      LUMINANCE
      LUMINANCE_ALPHA
      RGB
      RGBA
*/
int textureDataTypeToGLType(std::shared_ptr<ImageSemantic> s_, std::shared_ptr<TextureDataType> d_) {
  // TODO
  switch (s_->tag) {
    case ImageSemantic::tag::Color:
      return GL_RGBA;
      break;
    case ImageSemantic::tag::Depth:
      return GL_DEPTH_COMPONENT;
      break;
    case ImageSemantic::tag::Stencil:
      break;
  }
  throw "FIXME: This texture format is not yet supported";
}

int textureDataTypeToGLArityType(std::shared_ptr<ImageSemantic> s_, std::shared_ptr<TextureDataType> d_) {
  // TODO
  switch (s_->tag) {
    case ImageSemantic::tag::Color:
      return GL_RGBA;
      break;
    case ImageSemantic::tag::Depth:
      return GL_DEPTH_COMPONENT;
      break;
    case ImageSemantic::tag::Stencil:
      break;
  }
  throw "FIXME: This texture format is not yet supported";
}

int edgeMode(std::shared_ptr<::EdgeMode> e) {
  switch (e->tag) {
    case EdgeMode::tag::ClampToEdge:    return GL_CLAMP_TO_EDGE;
    case EdgeMode::tag::Repeat:         return GL_REPEAT;
    case EdgeMode::tag::MirroredRepeat: return GL_MIRRORED_REPEAT;
    default:
      throw "unsupported edge mode";
  }
}

int filterMode(std::shared_ptr<::Filter> f) {
  switch (f->tag) {
    case Filter::tag::Nearest:              return GL_NEAREST;
    case Filter::tag::Linear:               return GL_LINEAR;
    case Filter::tag::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
    case Filter::tag::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
    case Filter::tag::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
    case Filter::tag::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
    default:
      throw "unsupported filter mode";
  }
}

Texture createTexture(std::shared_ptr<TextureDescriptor> tx_) {
  Texture t;
  glGenTextures(1,&t.texture);
  auto tx = std::static_pointer_cast<data::TextureDescriptor>(tx_);
  auto size = std::static_pointer_cast<data::VV2U>(tx->textureSize);
  int width = size->_0.x;
  int height = size->_0.y;
  int internalFormat,dataFormat;
  switch (tx->textureType->tag) {
    case TextureType::tag::Texture2D: {
      t.target = GL_TEXTURE_2D;
      auto tx2D = std::static_pointer_cast<data::Texture2D>(tx->textureType);
      internalFormat = textureDataTypeToGLType(tx->textureSemantic,tx2D->_0);
      dataFormat = textureDataTypeToGLArityType(tx->textureSemantic,tx2D->_0);
      break;
    }
    case TextureType::tag::TextureCube: {
      t.target = GL_TEXTURE_CUBE_MAP;
      auto txCube = std::static_pointer_cast<data::TextureCube>(tx->textureType);
      internalFormat = textureDataTypeToGLType(tx->textureSemantic,txCube->_0);
      dataFormat = textureDataTypeToGLArityType(tx->textureSemantic,txCube->_0);
      break;
    }
    default:
      throw "unsupported texture type";
  }

  glBindTexture(t.target, t.texture);
  int dataType = (dataFormat == GL_DEPTH_COMPONENT)?GL_UNSIGNED_SHORT:GL_UNSIGNED_BYTE;
  for (int level = tx->textureBaseLevel; level <= tx->textureMaxLevel; level++) {
    if (t.target == GL_TEXTURE_2D) {
      glTexImage2D(t.target,level,internalFormat, width, height, 0, dataFormat, dataType, nullptr);
    } else {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,level,internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,level,internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,level,internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,level,internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,level,internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,level,internalFormat, width, height, 0, dataFormat, dataType, nullptr);
    }
    width /= 2;
    height /= 2;
  }

  // setup texture sampling
  auto s = std::static_pointer_cast<data::SamplerDescriptor>(tx->textureSampler);
  glTexParameteri(t.target, GL_TEXTURE_WRAP_S, edgeMode(s->samplerWrapS));
  glTexParameteri(t.target, GL_TEXTURE_WRAP_T, edgeMode(s->samplerWrapT.data));
  glTexParameteri(t.target, GL_TEXTURE_MIN_FILTER, filterMode(s->samplerMinFilter));
  glTexParameteri(t.target, GL_TEXTURE_MAG_FILTER, filterMode(s->samplerMagFilter));
  return t;
}

unsigned int GLES20Pipeline::createRenderTarget(std::shared_ptr<RenderTarget> t_) {
  auto t = std::static_pointer_cast<data::RenderTarget>(t_);
  // does this target have texture attachments?
  int textureCount = 0;
  for (auto i_ : t->renderTargets) {
    auto i = std::static_pointer_cast<data::TargetItem>(i_);
    if (i->targetRef.valid && i->targetRef.data->tag == ImageRef::tag::TextureImage)
      textureCount++;
  }
  if (textureCount == 0)
    return 0;

  // has textures attachment
  unsigned int fb;
  glGenFramebuffers(1,&fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  /*
  void FramebufferTexture2D( enum target, enum attachment,enum textarget, uint texture, int level );
    attachment:
      COLOR_ATTACHMENT0
      DEPTH_ATTACHMENT
      STENCIL_ATTACHMENT
  */
  int attachment,textarget,level;
  unsigned int texture;
  for (auto i_ : t->renderTargets) {
    auto i = std::static_pointer_cast<data::TargetItem>(i_);
    switch (i->targetSemantic->tag) {
      case ImageSemantic::tag::Color:
        attachment = GL_COLOR_ATTACHMENT0;
        break;
      case ImageSemantic::tag::Depth:
        attachment = GL_DEPTH_ATTACHMENT;
        break;
      case ImageSemantic::tag::Stencil:
        attachment = GL_STENCIL_ATTACHMENT;
        break;
    }
/*
    GL_TEXTURE_2D
    GL_TEXTURE_CUBE_MAP_POSITIVE_X
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
*/
    if (i->targetRef.valid) {
      switch (i->targetRef.data->tag) {
        case ImageRef::tag::TextureImage: {
          auto ti = std::static_pointer_cast<data::TextureImage>(i->targetRef.data);
          texture = textures[ti->_0].texture;
          textarget = GL_TEXTURE_2D; // TODO
          level = ti->_1;
          break;
        }
        case ImageRef::tag::Framebuffer:
          texture = 0;
          textarget = GL_TEXTURE_2D;
          level = 0;
          break;
      }
    } else {
      texture = 0;
      textarget = GL_TEXTURE_2D;
      level = 0;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER,attachment,textarget,texture,level);
  }
  return fb;
}

std::shared_ptr<GLStreamData> createStreamData(std::shared_ptr<StreamData> s_) {
  auto s = std::static_pointer_cast<data::StreamData>(s_);
  std::shared_ptr<GLStreamData> gls(new GLStreamData());

  switch (s->streamPrimitive->tag) {
    case FetchPrimitive::tag::Points:    gls->glMode = GL_POINTS;    break;
    case FetchPrimitive::tag::Lines:     gls->glMode = GL_LINES;     break;
    case FetchPrimitive::tag::Triangles: gls->glMode = GL_TRIANGLES; break;
  }
  auto buffer = std::shared_ptr<Buffer>(new Buffer());
  for (auto i : s->streamData) {
    switch (i.second->tag) {
      case ArrayValue::tag::VBoolArray: {
        auto a = std::static_pointer_cast<data::VBoolArray>(i.second);
        // TODO
      }
      break;
      case ArrayValue::tag::VIntArray: {
        auto a = std::static_pointer_cast<data::VIntArray>(i.second);
        // TODO
        //auto type = inputType(s->streamType[i.first]);
        //gls->streams.add(i.first, type, buffer, buffer->add(a->_0));
      }
      break;
      case ArrayValue::tag::VWordArray: {
        auto a = std::static_pointer_cast<data::VWordArray>(i.second);
        // TODO
        //auto type = inputType(s->streamType[i.first]);
        //gls->streams.add(i.first, type, buffer, buffer->add(a->_0));
      }
      break;
      case ArrayValue::tag::VFloatArray: {
        auto a = std::static_pointer_cast<data::VFloatArray>(i.second);
        auto type = inputType(s->streamType[i.first]);
        gls->streams.add(i.first, type, buffer, buffer->add(a->_0));
      }
      break;
    }
  }
  buffer->freeze();
  gls->streams.validate(); // TODO

  gls->glCount = 0;
  for (auto i : gls->streams.map) {
    if (i.second->isArray) {
      gls->glCount = i.second->buffer->size[i.second->index] / i.second->glSize;
      break;
    }
  }
  return gls;
}

std::shared_ptr<GLProgram> createProgram(std::shared_ptr<Program> p_) {
  auto p = std::static_pointer_cast<data::Program>(p_);
  // vertex shader
  unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
  const char* vsSrc = p->vertexShader.c_str();
  glShaderSource(vs,1,&vsSrc,nullptr);
  glCompileShader(vs);

  // fragment shader
  unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fsSrc = p->fragmentShader.c_str();
  glShaderSource(fs,1,&fsSrc,nullptr);
  glCompileShader(fs);

  // create program
  unsigned int po = glCreateProgram();
  glAttachShader(po,vs);
  glAttachShader(po,fs);
  glLinkProgram(po);

  std::shared_ptr<GLProgram> glp(new GLProgram());
  glp->program = po;
  glp->vertexShader = vs;
  glp->fragmentShader = fs;

  // query uniforms
  int loc;
  for(auto const &i : p->programUniforms) {
    loc = glGetUniformLocation(po, i.first.c_str());
    if (loc >= 0) {
      glp->programUniforms[i.first] = loc;
    }
  }
  // query sampler uniforms
  for(auto const &i : p->programInTextures) {
    loc = glGetUniformLocation(po, i.first.c_str());
    if (loc >= 0) {
      glp->programInTextures[i.first] = loc;
    }
  }
  // query vertex attributes
  for(auto const &i : p->programStreams) {
    loc = glGetAttribLocation(po, i.first.c_str());
    if (loc >= 0) {
      auto param = std::static_pointer_cast<data::Parameter>(i.second);
      glp->programStreams[i.first] = {.name = param->name, .index = loc};
    }
  }
  return glp;
}

GLES20Pipeline::GLES20Pipeline(std::shared_ptr<Pipeline> ppl_) {
  screenTarget = 0;
  hasCurrentProgram = false;
  auto ppl = std::static_pointer_cast<data::Pipeline>(ppl_);
  pipeline = ppl;
  // check backend compatibility
  if (ppl->backend->tag != Backend::tag::WebGL1) {
    throw "unsupported backend";
  }
  // allocate all resources
  //  textures
  for (auto i : ppl->textures) {
    textures.push_back(createTexture(i));
  }
  //  targets
  for (auto i : ppl->targets) {
    targets.push_back(createRenderTarget(i));
  }
  //  programs
  for (auto i : ppl->programs) {
    programs.push_back(createProgram(i));
  }
  //  stream data
  for (auto i : ppl->streams) {
    streamData.push_back(createStreamData(i));
  }
  glReleaseShaderCompiler();
}

GLES20Pipeline::~GLES20Pipeline() {
  // release resources
  // textures
  for (auto i : textures) {
    glDeleteTextures(1,&i.texture);
  }
  // targets
  for (auto i : targets) {
    glDeleteFramebuffers(1,&i);
  }
  // programs
  for (auto i : programs) {
    glDeleteProgram(i->program);
    glDeleteShader(i->vertexShader);
    glDeleteShader(i->fragmentShader);
  }
}

void GLES20Pipeline::setPipelineInput(std::shared_ptr<PipelineInput> i) {
  input = i;
}

void setupRasterContext(std::shared_ptr<RasterContext> ctx_) {
  switch (ctx_->tag) {
    case RasterContext::tag::PointCtx: {
      auto ctx = std::static_pointer_cast<data::PointCtx>(ctx_);
      switch (ctx->_0->tag) {
        case PointSize::tag::ProgramPointSize:
          break;
        default:
          throw "unsupported point size";
      }
      break;
    }
    case RasterContext::tag::LineCtx: {
      auto ctx = std::static_pointer_cast<data::LineCtx>(ctx_);
      glLineWidth(ctx->_0);
      break;
    }
    case RasterContext::tag::TriangleCtx: {
      auto ctx = std::static_pointer_cast<data::TriangleCtx>(ctx_);
      switch (ctx->_0->tag) {
        case CullMode::tag::CullNone:
          glDisable(GL_CULL_FACE);
          break;
        case CullMode::tag::CullFront: {
          auto f = std::static_pointer_cast<data::CullFront>(ctx->_0);
          glEnable(GL_CULL_FACE);
          glCullFace(GL_FRONT);
          glFrontFace(frontFace(f->_0));
          break;
        }
        case CullMode::tag::CullBack: {
          auto f = std::static_pointer_cast<data::CullBack>(ctx->_0);
          glEnable(GL_CULL_FACE);
          glCullFace(GL_BACK);
          glFrontFace(frontFace(f->_0));
          break;
        }
      }
      glDisable(GL_POLYGON_OFFSET_FILL);
      switch (ctx->_2->tag) {
        case PolygonOffset::tag::NoOffset:
          break;
        case PolygonOffset::tag::Offset: {
          auto o = std::static_pointer_cast<data::Offset>(ctx->_2);
          glPolygonOffset(o->_0,o->_1);
          glEnable(GL_POLYGON_OFFSET_FILL);
          break;
        }
      }
      break;
    }
  }
}

void setupAccumulationContext(std::shared_ptr<AccumulationContext> ctx_) {
  auto ctx = std::static_pointer_cast<data::AccumulationContext>(ctx_);
  bool noDepth = true;
  bool noStencil = true;
  bool noColor = true;
  for (auto i : ctx->accOperations) {
    switch (i->tag) {
      case FragmentOperation::tag::DepthOp: {
        auto o = std::static_pointer_cast<data::DepthOp>(i);
        noDepth = false;
        int df = comparisonFunction(o->_0);
        if (df == GL_ALWAYS && o->_1 == false) {
          glDisable(GL_DEPTH_TEST);
        } else {
          glEnable(GL_DEPTH_TEST);
          glDepthFunc(df);
          glDepthMask(o->_1);
        }
        break;
      }
      case FragmentOperation::tag::StencilOp: {
        auto o = std::static_pointer_cast<data::StencilOp>(i);
        noStencil = false;
        break;
      }
      case FragmentOperation::tag::ColorOp: {
        auto o = std::static_pointer_cast<data::ColorOp>(i);
        noColor = false;
        switch (o->_0->tag) {
          case Blending::tag::NoBlending:
            glDisable(GL_BLEND);
            break;
          case Blending::tag::BlendLogicOp:
            glDisable(GL_BLEND);
            break;
          case Blending::tag::Blend: {
            auto b = std::static_pointer_cast<data::Blend>(o->_0);
            glEnable(GL_BLEND);
            glBlendEquationSeparate(blendEquation(b->colorEqSrc),blendEquation(b->alphaEqSrc));
            glBlendColor(b->color.x,b->color.y,b->color.z,b->color.w);
            glBlendFuncSeparate(blendingFactor(b->colorFSrc),blendingFactor(b->colorFDst)
                               ,blendingFactor(b->alphaFSrc),blendingFactor(b->alphaFDst));
            break;
          }
        }
        bool maskR = true;
        bool maskG = true;
        bool maskB = true;
        bool maskA = true;
        switch (o->_1->tag) {
          case Value::tag::VBool: {
            auto v = std::static_pointer_cast<data::VBool>(o->_1);
            maskR = v->_0;
            break;
          }
          case Value::tag::VV2B: {
            auto v = std::static_pointer_cast<data::VV2B>(o->_1);
            maskR = v->_0.x;
            maskG = v->_0.y;
            break;
          }
          case Value::tag::VV3B: {
            auto v = std::static_pointer_cast<data::VV3B>(o->_1);
            maskR = v->_0.x;
            maskG = v->_0.y;
            maskB = v->_0.z;
            break;
          }
          case Value::tag::VV4B: {
            auto v = std::static_pointer_cast<data::VV4B>(o->_1);
            maskR = v->_0.x;
            maskG = v->_0.y;
            maskB = v->_0.z;
            maskA = v->_0.w;
            break;
          }
        }
        glColorMask(maskR,maskG,maskB,maskA);
        break;
      }
    }
  }
  if (noStencil) {
    glDisable(GL_STENCIL_TEST);
  }
  if (noDepth) {
    glDisable(GL_DEPTH_TEST);
  }
}

void GLES20Pipeline::render() {
  //std::cout << "render\n";
  for (auto i : pipeline->commands) {
    switch (i->tag) {
      case Command::tag::SetRasterContext: {
        auto cmd = std::static_pointer_cast<data::SetRasterContext>(i);
        setupRasterContext(cmd->_0);
        break;
      }
      case Command::tag::SetAccumulationContext: {
        auto cmd = std::static_pointer_cast<data::SetAccumulationContext>(i);
        setupAccumulationContext(cmd->_0);
        break;
      }
      case Command::tag::SetTexture: {
        auto cmd = std::static_pointer_cast<data::SetTexture>(i);
        glActiveTexture(GL_TEXTURE0 + cmd->_0);
        glBindTexture(textures[cmd->_1].target,textures[cmd->_1].texture);
        break;
      }
      case Command::tag::SetProgram: {
        auto cmd = std::static_pointer_cast<data::SetProgram>(i);
        hasCurrentProgram = true;
        currentProgram = cmd->_0;
        glUseProgram(programs[currentProgram]->program);
        break;
      }
      case Command::tag::SetRenderTarget: {
        auto cmd = std::static_pointer_cast<data::SetRenderTarget>(i);
        unsigned int t = targets[cmd->_0];
        glBindFramebuffer(GL_FRAMEBUFFER, (t==0)?screenTarget:t);
        if (input) {
          glViewport(0,0,input->screenWidth,input->screenHeight);
        }
        break;
      }
      case Command::tag::ClearRenderTarget: {
        auto cmd = std::static_pointer_cast<data::ClearRenderTarget>(i);
        unsigned int mask = 0;
        for (auto a : cmd->_0) {
          auto image = std::static_pointer_cast<data::ClearImage>(a);
          switch (image->imageSemantic->tag) {
            case ImageSemantic::tag::Depth: {
              auto v = std::static_pointer_cast<data::VFloat>(image->clearValue);
              glDepthMask(true);
              glClearDepthf(v->_0);
              mask |= GL_DEPTH_BUFFER_BIT;
              break;
            }
            case ImageSemantic::tag::Stencil: {
              auto v = std::static_pointer_cast<data::VWord>(image->clearValue);
              glClearStencil(v->_0);
              mask |= GL_STENCIL_BUFFER_BIT;
              break;
            }
            case ImageSemantic::tag::Color:
              switch (image->clearValue->tag) {
                case Value::tag::VFloat: {
                  auto v = std::static_pointer_cast<data::VFloat>(image->clearValue);
                  glClearColor(v->_0,0.0,0.0,1.0);
                  break;
                }
                case Value::tag::VV2F: {
                  auto v = std::static_pointer_cast<data::VV2F>(image->clearValue);
                  glClearColor(v->_0.x,v->_0.y,0.0,1.0);
                  break;
                }
                case Value::tag::VV3F: {
                  auto v = std::static_pointer_cast<data::VV3F>(image->clearValue);
                  glClearColor(v->_0.x,v->_0.y,v->_0.z,1.0);
                  break;
                }
                case Value::tag::VV4F: {
                  auto v = std::static_pointer_cast<data::VV4F>(image->clearValue);
                  glClearColor(v->_0.x,v->_0.y,v->_0.z,v->_0.w);
                  break;
                }
                default:
                  glClearColor(0.0,0.0,0.0,1.0);
              }
              glColorMask(true,true,true,true);
              mask |= GL_COLOR_BUFFER_BIT;
              break;
          }
        }
        glClear(mask);
        break;
      }
      case Command::tag::SetSamplerUniform: if (hasCurrentProgram) {
        auto cmd = std::static_pointer_cast<data::SetSamplerUniform>(i);
        int sampler = programs[currentProgram]->programInTextures[cmd->_0];
        glUniform1i(sampler,cmd->_1);
        break;
      }
      case Command::tag::RenderSlot: if (input && pipeline && hasCurrentProgram) {
        auto cmd = std::static_pointer_cast<data::RenderSlot>(i);
        auto slot = std::static_pointer_cast<data::Slot>(pipeline->slots[cmd->_0]);
        if (input->objectMap.count(slot->slotName)<=0)
          break;
        for (auto o: *input->objectMap[slot->slotName]) {
          if (!o->enabled) continue;
          // setup uniforms
          for (auto u: programs[currentProgram]->programUniforms) {
            if (o->uniforms.count(u.first)>0) {
              setUniformValue(u.second,o->uniforms[u.first]);
            } else {
              setUniformValue(u.second,input->uniforms[u.first]);
            }
          }
          // setup streams
          for (auto s: programs[currentProgram]->programStreams) {
            setStream(s.second.index,*o->streams->map[s.second.name]);
          }
          // draw call
          // TODO: support index buffers
          glDrawArrays(o->glMode, 0, o->glCount);
        }
        break;
      }
      case Command::tag::RenderStream: if (input && pipeline && hasCurrentProgram) {
        auto cmd = std::static_pointer_cast<data::RenderStream>(i);
        auto data = streamData[cmd->_0];
        // setup streams
        for (auto s: programs[currentProgram]->programStreams) {
          setStream(s.second.index,*data->streams.map[s.second.name]);
        }
        // draw call
        // TODO: support index buffers
        glDrawArrays(data->glMode, 0, data->glCount);
        break;
      }

      // unused commands
      case Command::tag::GenerateMipMap:
      case Command::tag::SetSampler:
      case Command::tag::SaveImage:
      case Command::tag::LoadImage:
        break;
    }
  }
}
