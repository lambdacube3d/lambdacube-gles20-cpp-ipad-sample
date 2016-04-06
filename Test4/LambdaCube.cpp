#include <iostream>
#include <OpenGLES/ES2/gl.h>
#include "LambdaCube.hpp"

enum Type inputType(std::shared_ptr<InputType> t) {
  switch (t->tag) {
    case ::InputType::tag::Float: return ::Type::FLOAT;
    case ::InputType::tag::V2F: return ::Type::FLOAT_VEC2;
    case ::InputType::tag::V3F: return ::Type::FLOAT_VEC3;
    case ::InputType::tag::V4F: return ::Type::FLOAT_VEC4;
    case ::InputType::tag::M22F: return ::Type::FLOAT_MAT2;
    case ::InputType::tag::M33F: return ::Type::FLOAT_MAT3;
    case ::InputType::tag::M44F: return ::Type::FLOAT_MAT4;
  }
  throw "illegal input type";
}

int32_t primitiveMode(enum Primitive p) {
  switch (p) {
    case ::Primitive::TriangleStrip: return GL_TRIANGLE_STRIP;
    case ::Primitive::TriangleList: return GL_TRIANGLES;
    case ::Primitive::TriangleFan: return GL_TRIANGLE_FAN;
    case ::Primitive::LineStrip: return GL_LINE_STRIP;
    case ::Primitive::LineList: return GL_LINES;
    case ::Primitive::LineLoop: return GL_LINE_LOOP;
    case ::Primitive::PointList: return GL_POINTS;
  }
  throw "illegal primitive mode";
}

int32_t blendingFactor(std::shared_ptr<BlendingFactor> bf) {
  switch (bf->tag) {
    case ::BlendingFactor::tag::ConstantAlpha: return GL_CONSTANT_ALPHA;
    case ::BlendingFactor::tag::ConstantColor: return GL_CONSTANT_COLOR;
    case ::BlendingFactor::tag::DstAlpha: return GL_DST_ALPHA;
    case ::BlendingFactor::tag::DstColor: return GL_DST_COLOR;
    case ::BlendingFactor::tag::One: return GL_ONE;
    case ::BlendingFactor::tag::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
    case ::BlendingFactor::tag::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
    case ::BlendingFactor::tag::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
    case ::BlendingFactor::tag::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
    case ::BlendingFactor::tag::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
    case ::BlendingFactor::tag::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
    case ::BlendingFactor::tag::SrcAlpha: return GL_SRC_ALPHA;
    case ::BlendingFactor::tag::SrcAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
    case ::BlendingFactor::tag::SrcColor: return GL_SRC_COLOR;
    case ::BlendingFactor::tag::Zero: return GL_ZERO;
  }
  throw "illegal blending factor";
}

int32_t blendEquation(std::shared_ptr<BlendEquation> be) {
  switch (be->tag) {
    case ::BlendEquation::tag::FuncAdd: return GL_FUNC_ADD;
    case ::BlendEquation::tag::FuncReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
    case ::BlendEquation::tag::FuncSubtract: return GL_FUNC_SUBTRACT;
  }
  throw "illegal blend equation";
}

int32_t comparisonFunction(std::shared_ptr<ComparisonFunction> cf) {
  switch (cf->tag) {
    case ::ComparisonFunction::tag::Always: return GL_ALWAYS;
    case ::ComparisonFunction::tag::Equal: return GL_EQUAL;
    case ::ComparisonFunction::tag::Gequal: return GL_GEQUAL;
    case ::ComparisonFunction::tag::Greater: return GL_GREATER;
    case ::ComparisonFunction::tag::Lequal: return GL_LEQUAL;
    case ::ComparisonFunction::tag::Less: return GL_LESS;
    case ::ComparisonFunction::tag::Never: return GL_NEVER;
    case ::ComparisonFunction::tag::Notequal: return GL_NOTEQUAL;
  }
  throw "illegal comparison function";
}

int32_t frontFace(std::shared_ptr<FrontFace> ff) {
  switch (ff->tag) {
    case ::FrontFace::tag::CCW: return GL_CCW;
    case ::FrontFace::tag::CW: return GL_CW;
  }
  throw "illegal front face value";
}

int32_t textureDataTypeToGLType(std::shared_ptr<ImageSemantic> s_, std::shared_ptr<TextureDataType> d_) {
  switch (s_->tag) {
    case ::ImageSemantic::tag::Color: return GL_RGBA;
    case ::ImageSemantic::tag::Depth: return GL_DEPTH_COMPONENT;
  }
  throw "FIXME: This texture format is not yet supported";
}

int32_t textureDataTypeToGLArityType(std::shared_ptr<ImageSemantic> s_, std::shared_ptr<TextureDataType> d_) {
  switch (s_->tag) {
    case ::ImageSemantic::tag::Color: return GL_RGBA;
    case ::ImageSemantic::tag::Depth: return GL_DEPTH_COMPONENT;
  }
  throw "FIXME: This texture format is not yet supported";
}

int32_t edgeMode(std::shared_ptr<EdgeMode> e) {
  switch (e->tag) {
    case ::EdgeMode::tag::ClampToEdge: return GL_CLAMP_TO_EDGE;
    case ::EdgeMode::tag::Repeat: return GL_REPEAT;
    case ::EdgeMode::tag::MirroredRepeat: return GL_MIRRORED_REPEAT;
    default:
      throw "unsupported edge mode";

  }
}

int32_t filterMode(std::shared_ptr<Filter> f) {
  switch (f->tag) {
    case ::Filter::tag::Nearest: return GL_NEAREST;
    case ::Filter::tag::Linear: return GL_LINEAR;
    case ::Filter::tag::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
    case ::Filter::tag::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
    case ::Filter::tag::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
    case ::Filter::tag::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
    default:
      throw "unsupported filter mode";

  }
}

void setUniformValue(int32_t i, std::shared_ptr<UniformValue> v) {
  switch (v->tag) {
    case ::InputType::tag::Int: {
      glUniform1iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::Bool: {
      glUniform1iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::Float: {
      glUniform1fv(i, 1, v->_float);
      break;
    }
    case ::InputType::tag::V2I: {
      glUniform2iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::V2B: {
      glUniform2iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::V2F: {
      glUniform2fv(i, 1, v->_float);
      break;
    }
    case ::InputType::tag::V3I: {
      glUniform3iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::V3B: {
      glUniform3iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::V3F: {
      glUniform3fv(i, 1, v->_float);
      break;
    }
    case ::InputType::tag::V4I: {
      glUniform4iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::V4B: {
      glUniform4iv(i, 1, v->_int);
      break;
    }
    case ::InputType::tag::V4F: {
      glUniform4fv(i, 1, v->_float);
      break;
    }
    case ::InputType::tag::M22F: {
      glUniformMatrix2fv(i, 1, false, v->_float);
      break;
    }
    case ::InputType::tag::M33F: {
      glUniformMatrix3fv(i, 1, false, v->_float);
      break;
    }
    case ::InputType::tag::M44F: {
      glUniformMatrix4fv(i, 1, false, v->_float);
      break;
    }
  }
}

void setStream(int32_t i, std::shared_ptr<Stream> s) {
  if (s->isArray) {
    glBindBuffer(GL_ARRAY_BUFFER, s->buffer->bufferObject);
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, s->glSize, s->buffer->glType[s->index], false, 0, (const void*) s->buffer->offset[s->index]);
  } else {
    glDisableVertexAttribArray(i);
    switch (s->type) {
      case ::Type::FLOAT: {
        glVertexAttrib1fv(i, s->attributeValue->_float + 0);
        break;
      }
      case ::Type::FLOAT_VEC2: {
        glVertexAttrib2fv(i, s->attributeValue->_float + 0);
        break;
      }
      case ::Type::FLOAT_VEC3: {
        glVertexAttrib3fv(i, s->attributeValue->_float + 0);
        break;
      }
      case ::Type::FLOAT_VEC4: {
        glVertexAttrib4fv(i, s->attributeValue->_float + 0);
        break;
      }
      case ::Type::FLOAT_MAT2: {
        glVertexAttrib2fv(i, s->attributeValue->_float + 0);
        glVertexAttrib2fv(i + 1, s->attributeValue->_float + 2);
        break;
      }
      case ::Type::FLOAT_MAT3: {
        glVertexAttrib3fv(i, s->attributeValue->_float + 0);
        glVertexAttrib3fv(i + 1, s->attributeValue->_float + 3);
        glVertexAttrib3fv(i + 2, s->attributeValue->_float + 6);
        break;
      }
      case ::Type::FLOAT_MAT4: {
        glVertexAttrib4fv(i, s->attributeValue->_float + 0);
        glVertexAttrib4fv(i + 1, s->attributeValue->_float + 4);
        glVertexAttrib4fv(i + 2, s->attributeValue->_float + 8);
        glVertexAttrib4fv(i + 3, s->attributeValue->_float + 12);
        break;
      }
    }
  }
}

std::shared_ptr<Texture> createTexture(std::shared_ptr<TextureDescriptor> tx_) {
  std::shared_ptr<Texture> t(new Texture());
  { unsigned int glObj; glGenTextures(1, &glObj); t->texture = glObj; }
  auto tx = std::static_pointer_cast<data::TextureDescriptor>(tx_);
  auto size = std::static_pointer_cast<data::VV2U>(tx->textureSize);
  int32_t width = size->_0.x;
  int32_t height = size->_0.y;
  int32_t internalFormat, dataFormat;
  switch (tx->textureType->tag) {
    case ::TextureType::tag::Texture2D: {
      t->target = GL_TEXTURE_2D;
      auto tx2D = std::static_pointer_cast<data::Texture2D>(tx->textureType);
      internalFormat = textureDataTypeToGLType(tx->textureSemantic, tx2D->_0);
      dataFormat = textureDataTypeToGLArityType(tx->textureSemantic, tx2D->_0);
      break;
    }
    case ::TextureType::tag::TextureCube: {
      t->target = GL_TEXTURE_CUBE_MAP;
      auto txCube = std::static_pointer_cast<data::TextureCube>(tx->textureType);
      internalFormat = textureDataTypeToGLType(tx->textureSemantic, txCube->_0);
      dataFormat = textureDataTypeToGLArityType(tx->textureSemantic, txCube->_0);
      break;
    }
    default:
      throw "unsupported texture type";

  }
  glBindTexture(t->target, t->texture);
  int32_t dataType = dataFormat == GL_DEPTH_COMPONENT?GL_UNSIGNED_SHORT:GL_UNSIGNED_BYTE;
  for (int32_t level = tx->textureBaseLevel; level <= tx->textureMaxLevel; level++ ) {
    if (t->target == GL_TEXTURE_2D) {
      glTexImage2D(t->target, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
    } else {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, internalFormat, width, height, 0, dataFormat, dataType, nullptr);
    }
    width /= 2;
    height /= 2;
  }
  auto s = std::static_pointer_cast<data::SamplerDescriptor>(tx->textureSampler);
  glTexParameteri(t->target, GL_TEXTURE_WRAP_S, edgeMode(s->samplerWrapS));
  glTexParameteri(t->target, GL_TEXTURE_WRAP_T, edgeMode(s->samplerWrapT.data));
  glTexParameteri(t->target, GL_TEXTURE_MIN_FILTER, filterMode(s->samplerMinFilter));
  glTexParameteri(t->target, GL_TEXTURE_MAG_FILTER, filterMode(s->samplerMagFilter));
  return t;
}

std::shared_ptr<GLStreamData> createStreamData(std::shared_ptr<StreamData> s_) {
  auto s = std::static_pointer_cast<data::StreamData>(s_);
  std::shared_ptr<GLStreamData> gls(new GLStreamData());
  switch (s->streamPrimitive->tag) {
    case ::FetchPrimitive::tag::Points: gls->glMode = GL_POINTS; break;
    case ::FetchPrimitive::tag::Lines: gls->glMode = GL_LINES; break;
    case ::FetchPrimitive::tag::Triangles: gls->glMode = GL_TRIANGLES; break;
  }
  std::shared_ptr<GLBuffer> buffer(new GLBuffer());
  for (auto i : s->streamData) {
    switch (i.second->tag) {
      case ::ArrayValue::tag::VBoolArray: {
        auto a = std::static_pointer_cast<data::VBoolArray>(i.second);
        break;
      }
      case ::ArrayValue::tag::VIntArray: {
        auto a = std::static_pointer_cast<data::VIntArray>(i.second);
        break;
      }
      case ::ArrayValue::tag::VWordArray: {
        auto a = std::static_pointer_cast<data::VWordArray>(i.second);
        break;
      }
      case ::ArrayValue::tag::VFloatArray: {
        auto a = std::static_pointer_cast<data::VFloatArray>(i.second);
        enum Type type = inputType(s->streamType[i.first]);
        void* buf = a->_0.data();
        gls->streams->add(i.first, type, buffer, buffer->add(buf, GL_FLOAT, a->_0.size()));
        break;
      }
    }
  }
  buffer->freeze();
  gls->streams->validate();
  gls->glCount = 0;
  for (auto i : gls->streams->map) {
    if (i.second->isArray) {
      gls->glCount = i.second->buffer->size[i.second->index] / i.second->glSize;
      break;
    }
  }
  return gls;
}

std::shared_ptr<GLProgram> createProgram(std::shared_ptr<Program> p_) {
  auto p = std::static_pointer_cast<data::Program>(p_);
  uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
  { const char* glslSrc = p->vertexShader.c_str(); glShaderSource(vs, 1, &glslSrc, 0); }
  glCompileShader(vs);
  uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
  { const char* glslSrc = p->fragmentShader.c_str(); glShaderSource(fs, 1, &glslSrc, 0); }
  glCompileShader(fs);
  uint32_t po = glCreateProgram();
  glAttachShader(po, vs);
  glAttachShader(po, fs);
  glLinkProgram(po);
  std::shared_ptr<GLProgram> glp(new GLProgram());
  glp->program = po;
  glp->vertexShader = vs;
  glp->fragmentShader = fs;
  int32_t loc;
  for (auto i : p->programUniforms) {
    loc = glGetUniformLocation(po, i.first.c_str());
    if (loc >= 0) {
      glp->programUniforms[i.first] = loc;
    }
  }
  for (auto i : p->programInTextures) {
    loc = glGetUniformLocation(po, i.first.c_str());
    if (loc >= 0) {
      glp->programInTextures[i.first] = loc;
    }
  }
  for (auto i : p->programStreams) {
    loc = glGetAttribLocation(po, i.first.c_str());
    if (loc >= 0) {
      auto param = std::static_pointer_cast<data::Parameter>(i.second);
      std::shared_ptr<StreamInfo> s(new StreamInfo());
      s->name = param->name;
      s->index = loc;
      glp->programStreams[i.first] = s;
    }
  }
  return glp;
}

void setupRasterContext(std::shared_ptr<RasterContext> ctx_) {
  switch (ctx_->tag) {
    case ::RasterContext::tag::PointCtx: {
      auto ctx = std::static_pointer_cast<data::PointCtx>(ctx_);
      switch (ctx->_0->tag) {
        case ::PointSize::tag::ProgramPointSize: {
          break;
        }
        default:
          throw "unsupported point size";

      }
      break;
    }
    case ::RasterContext::tag::LineCtx: {
      auto ctx = std::static_pointer_cast<data::LineCtx>(ctx_);
      glLineWidth(ctx->_0);
      break;
    }
    case ::RasterContext::tag::TriangleCtx: {
      auto ctx = std::static_pointer_cast<data::TriangleCtx>(ctx_);
      switch (ctx->_0->tag) {
        case ::CullMode::tag::CullNone: glDisable(GL_CULL_FACE); break;
        case ::CullMode::tag::CullFront: {
          auto f = std::static_pointer_cast<data::CullFront>(ctx->_0);
          glEnable(GL_CULL_FACE);
          glCullFace(GL_FRONT);
          glFrontFace(frontFace(f->_0));
          break;
        }
        case ::CullMode::tag::CullBack: {
          auto f = std::static_pointer_cast<data::CullBack>(ctx->_0);
          glEnable(GL_CULL_FACE);
          glCullFace(GL_BACK);
          glFrontFace(frontFace(f->_0));
          break;
        }
      }
      glDisable(GL_POLYGON_OFFSET_FILL);
      switch (ctx->_2->tag) {
        case ::PolygonOffset::tag::NoOffset: {
          break;
        }
        case ::PolygonOffset::tag::Offset: {
          auto o = std::static_pointer_cast<data::Offset>(ctx->_2);
          glPolygonOffset(o->_0, o->_1);
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
      case ::FragmentOperation::tag::DepthOp: {
        auto o = std::static_pointer_cast<data::DepthOp>(i);
        noDepth = false;
        int32_t df = comparisonFunction(o->_0);
        if (df == GL_ALWAYS && o->_1 == false) {
          glDisable(GL_DEPTH_TEST);
        } else {
          glEnable(GL_DEPTH_TEST);
          glDepthFunc(df);
          glDepthMask(o->_1);
        }
        break;
      }
      case ::FragmentOperation::tag::StencilOp: {
        auto o = std::static_pointer_cast<data::StencilOp>(i);
        noStencil = false;
        break;
      }
      case ::FragmentOperation::tag::ColorOp: {
        auto o = std::static_pointer_cast<data::ColorOp>(i);
        noColor = false;
        switch (o->_0->tag) {
          case ::Blending::tag::NoBlending: glDisable(GL_BLEND); break;
          case ::Blending::tag::BlendLogicOp: glDisable(GL_BLEND); break;
          case ::Blending::tag::Blend: {
            auto b = std::static_pointer_cast<data::Blend>(o->_0);
            glEnable(GL_BLEND);
            glBlendEquationSeparate(blendEquation(b->colorEqSrc), blendEquation(b->alphaEqSrc));
            glBlendColor(b->color.x, b->color.y, b->color.z, b->color.w);
            glBlendFuncSeparate(blendingFactor(b->colorFSrc), blendingFactor(b->colorFDst), blendingFactor(b->alphaFSrc), blendingFactor(b->alphaFDst));
            break;
          }
        }
        bool maskR = true;
        bool maskG = true;
        bool maskB = true;
        bool maskA = true;
        switch (o->_1->tag) {
          case ::Value::tag::VBool: {
            auto v = std::static_pointer_cast<data::VBool>(o->_1);
            maskR = v->_0;
            break;
          }
          case ::Value::tag::VV2B: {
            auto v = std::static_pointer_cast<data::VV2B>(o->_1);
            maskR = v->_0.x;
            maskG = v->_0.y;
            break;
          }
          case ::Value::tag::VV3B: {
            auto v = std::static_pointer_cast<data::VV3B>(o->_1);
            maskR = v->_0.x;
            maskG = v->_0.y;
            maskB = v->_0.z;
            break;
          }
          case ::Value::tag::VV4B: {
            auto v = std::static_pointer_cast<data::VV4B>(o->_1);
            maskR = v->_0.x;
            maskG = v->_0.y;
            maskB = v->_0.z;
            maskA = v->_0.w;
            break;
          }
        }
        glColorMask(maskR, maskG, maskB, maskA);
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

GLBuffer::GLBuffer() {
  
  
  
  
  
  

}

int32_t GLBuffer::add(const void* buf, int32_t elemGLType, int32_t elemCount) {
  int32_t i = data.size();
  int32_t elemSize = 1;
  switch (elemGLType) {
    case GL_UNSIGNED_BYTE: elemSize = 1; break;
    case GL_BYTE: elemSize = 1; break;
    case GL_UNSIGNED_SHORT: elemSize = 2; break;
    case GL_SHORT: elemSize = 2; break;
    default:
      elemSize = 4;

  }
  data.push_back(buf);
  size.push_back(elemCount);
  byteSize.push_back(elemSize * elemCount);
  glType.push_back(elemGLType);
  return i;
}

void GLBuffer::freeze() {
  uint32_t bufferSize = 0;
  for (auto i : byteSize) {
    offset.push_back(bufferSize);
    bufferSize += i;
  }
  uint32_t bo;
  { unsigned int glObj; glGenBuffers(1, &glObj); bo = glObj; }
  glBindBuffer(GL_ARRAY_BUFFER, bo);
  glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
  uint32_t offset_ = 0;
  for (int i = 0; i < data.size(); i++) {
    glBufferSubData(GL_ARRAY_BUFFER, offset_, byteSize[i], data[i]);
    offset_ += byteSize[i];
  }
  bufferObject = bo;
}

void GLBuffer::update(int32_t i, std::vector<float> v) {
}

Stream::Stream(float v) {
  type = ::Type::FLOAT;
  isArray = false;
  glSize = 1;
}

Stream::Stream(V2F v) {
  type = ::Type::FLOAT_VEC2;
  isArray = false;
  glSize = 2;
}

Stream::Stream(V3F v) {
  type = ::Type::FLOAT_VEC3;
  isArray = false;
  glSize = 3;
}

Stream::Stream(V4F v) {
  type = ::Type::FLOAT_VEC4;
  isArray = false;
  glSize = 4;
}

Stream::Stream(M22F v) {
  type = ::Type::FLOAT_MAT2;
  isArray = false;
  glSize = 4;
}

Stream::Stream(M33F v) {
  type = ::Type::FLOAT_MAT3;
  isArray = false;
  glSize = 9;
}

Stream::Stream(M44F v) {
  type = ::Type::FLOAT_MAT4;
  isArray = false;
  glSize = 16;
}

Stream::Stream(std::shared_ptr<GLBuffer> b, int32_t i, enum Type t) {
  type = t;
  buffer = b;
  index = i;
  isArray = true;
  glSize = 16;
  switch (t) {
    case ::Type::FLOAT: glSize = 1; break;
    case ::Type::FLOAT_VEC2: glSize = 2; break;
    case ::Type::FLOAT_VEC3: glSize = 3; break;
    case ::Type::FLOAT_VEC4: glSize = 4; break;
    case ::Type::FLOAT_MAT2: glSize = 4; break;
    case ::Type::FLOAT_MAT3: glSize = 9; break;
    case ::Type::FLOAT_MAT4: glSize = 16; break;
  }
}

StreamMap::StreamMap() {
  
  

}

void StreamMap::add(std::string name, float v) {
  std::shared_ptr<Stream> s(new Stream(v));
  map[name] = s;
}

void StreamMap::add(std::string name, V2F v) {
  std::shared_ptr<Stream> s(new Stream(v));
  map[name] = s;
}

void StreamMap::add(std::string name, V3F v) {
  std::shared_ptr<Stream> s(new Stream(v));
  map[name] = s;
}

void StreamMap::add(std::string name, V4F v) {
  std::shared_ptr<Stream> s(new Stream(v));
  map[name] = s;
}

void StreamMap::add(std::string name, M22F v) {
  std::shared_ptr<Stream> s(new Stream(v));
  map[name] = s;
}

void StreamMap::add(std::string name, M33F v) {
  std::shared_ptr<Stream> s(new Stream(v));
  map[name] = s;
}

void StreamMap::add(std::string name, M44F v) {
  std::shared_ptr<Stream> s(new Stream(v));
  map[name] = s;
}

void StreamMap::add(std::string name, enum Type t, std::shared_ptr<GLBuffer> b, int32_t index) {
  std::shared_ptr<Stream> s(new Stream(b, index, t));
  map[name] = s;
}

bool StreamMap::validate() {
  return true;
}

GLObject::~GLObject() {
}

void GLObject::enable(bool visible) {
  enabled = visible;
}

void GLObject::setOrder(int32_t o) {
  order = o;
}

void GLObject::setUniform(std::string name, int32_t v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::Int;
    u->_int = new int[1];
    u->_int[0] = v;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, bool v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::Bool;
    u->_int = new int[1];
    u->_int[0] = v?1:0;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, float v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::Float;
    u->_float = new float[1];
    u->_float[0] = v;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V2I v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V2I;
    u->_int = new int[2];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V2B v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V2B;
    u->_int = new int[2];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V2F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V2F;
    u->_float = new float[2];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V3I v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V3I;
    u->_int = new int[3];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V3B v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V3B;
    u->_int = new int[3];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V3F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V3F;
    u->_float = new float[3];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V4I v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
    u->_int[3] = v.w;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V4I;
    u->_int = new int[4];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
    u->_int[3] = v.w;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V4B v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
    u->_int[3] = v.w?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V4B;
    u->_int = new int[4];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
    u->_int[3] = v.w?1:0;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, V4F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
    u->_float[3] = v.w;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V4F;
    u->_float = new float[4];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
    u->_float[3] = v.w;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, M22F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.y.x;
    u->_float[3] = v.y.y;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::M22F;
    u->_float = new float[4];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.y.x;
    u->_float[3] = v.y.y;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, M33F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.y.x;
    u->_float[4] = v.y.y;
    u->_float[5] = v.y.z;
    u->_float[6] = v.z.x;
    u->_float[7] = v.z.y;
    u->_float[8] = v.z.z;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::M33F;
    u->_float = new float[9];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.y.x;
    u->_float[4] = v.y.y;
    u->_float[5] = v.y.z;
    u->_float[6] = v.z.x;
    u->_float[7] = v.z.y;
    u->_float[8] = v.z.z;
    uniforms[name] = u;
  }
}

void GLObject::setUniform(std::string name, M44F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.x.w;
    u->_float[4] = v.y.x;
    u->_float[5] = v.y.y;
    u->_float[6] = v.y.z;
    u->_float[7] = v.y.w;
    u->_float[8] = v.z.x;
    u->_float[9] = v.z.y;
    u->_float[10] = v.z.z;
    u->_float[11] = v.z.w;
    u->_float[12] = v.w.x;
    u->_float[13] = v.w.y;
    u->_float[14] = v.w.z;
    u->_float[15] = v.w.w;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::M44F;
    u->_float = new float[16];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.x.w;
    u->_float[4] = v.y.x;
    u->_float[5] = v.y.y;
    u->_float[6] = v.y.z;
    u->_float[7] = v.y.w;
    u->_float[8] = v.z.x;
    u->_float[9] = v.z.y;
    u->_float[10] = v.z.z;
    u->_float[11] = v.z.w;
    u->_float[12] = v.w.x;
    u->_float[13] = v.w.y;
    u->_float[14] = v.w.z;
    u->_float[15] = v.w.w;
    uniforms[name] = u;
  }
}

PipelineInput::PipelineInput() {
  
  
  

}

std::shared_ptr<GLObject> PipelineInput::createObject(std::string slotName, enum Primitive prim, std::shared_ptr<StreamMap> attributes, std::vector<std::string> objectUniforms) {
  std::shared_ptr<GLObject> o(new GLObject());
  o->enabled = true;
  o->order = 0;
  o->glMode = primitiveMode(prim);
  int32_t count = 0;
  for (auto i : attributes->map) {
    if (i.second->isArray) {
      count = i.second->buffer->size[i.second->index] / i.second->glSize;
      break;
    }
  }
  o->glCount = count;
  o->streams = attributes;
  if (objectMap.count(slotName)>0) {
    objectMap[slotName].push_back(o);
  } else {
    std::vector<std::shared_ptr<GLObject>> ov;
    ov.push_back(o);
    objectMap[slotName] = ov;
  }
  return o;
}

std::shared_ptr<GLObject> PipelineInput::createObject(std::string slotName, enum Primitive prim, std::shared_ptr<StreamMap> attributes, std::shared_ptr<GLBuffer> indexBuffer, int32_t bufferIndex, std::vector<std::string> objectUniforms) {
  std::shared_ptr<GLObject> o(new GLObject());
  return o;
}

void PipelineInput::sortSlotObjects() {
}

void PipelineInput::setScreenSize(int32_t w, int32_t h) {
  screenWidth = w;
  screenHeight = h;
}

void PipelineInput::setUniform(std::string name, int32_t v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::Int;
    u->_int = new int[1];
    u->_int[0] = v;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, bool v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::Bool;
    u->_int = new int[1];
    u->_int[0] = v?1:0;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, float v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::Float;
    u->_float = new float[1];
    u->_float[0] = v;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V2I v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V2I;
    u->_int = new int[2];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V2B v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V2B;
    u->_int = new int[2];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V2F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V2F;
    u->_float = new float[2];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V3I v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V3I;
    u->_int = new int[3];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V3B v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V3B;
    u->_int = new int[3];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V3F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V3F;
    u->_float = new float[3];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V4I v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
    u->_int[3] = v.w;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V4I;
    u->_int = new int[4];
    u->_int[0] = v.x;
    u->_int[1] = v.y;
    u->_int[2] = v.z;
    u->_int[3] = v.w;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V4B v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
    u->_int[3] = v.w?1:0;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V4B;
    u->_int = new int[4];
    u->_int[0] = v.x?1:0;
    u->_int[1] = v.y?1:0;
    u->_int[2] = v.z?1:0;
    u->_int[3] = v.w?1:0;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, V4F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
    u->_float[3] = v.w;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::V4F;
    u->_float = new float[4];
    u->_float[0] = v.x;
    u->_float[1] = v.y;
    u->_float[2] = v.z;
    u->_float[3] = v.w;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, M22F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.y.x;
    u->_float[3] = v.y.y;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::M22F;
    u->_float = new float[4];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.y.x;
    u->_float[3] = v.y.y;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, M33F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.y.x;
    u->_float[4] = v.y.y;
    u->_float[5] = v.y.z;
    u->_float[6] = v.z.x;
    u->_float[7] = v.z.y;
    u->_float[8] = v.z.z;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::M33F;
    u->_float = new float[9];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.y.x;
    u->_float[4] = v.y.y;
    u->_float[5] = v.y.z;
    u->_float[6] = v.z.x;
    u->_float[7] = v.z.y;
    u->_float[8] = v.z.z;
    uniforms[name] = u;
  }
}

void PipelineInput::setUniform(std::string name, M44F v) {
  if (uniforms.count(name)>0) {
    std::shared_ptr<UniformValue> u = uniforms[name];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.x.w;
    u->_float[4] = v.y.x;
    u->_float[5] = v.y.y;
    u->_float[6] = v.y.z;
    u->_float[7] = v.y.w;
    u->_float[8] = v.z.x;
    u->_float[9] = v.z.y;
    u->_float[10] = v.z.z;
    u->_float[11] = v.z.w;
    u->_float[12] = v.w.x;
    u->_float[13] = v.w.y;
    u->_float[14] = v.w.z;
    u->_float[15] = v.w.w;
  } else {
    std::shared_ptr<UniformValue> u(new UniformValue());
    u->tag = ::InputType::tag::M44F;
    u->_float = new float[16];
    u->_float[0] = v.x.x;
    u->_float[1] = v.x.y;
    u->_float[2] = v.x.z;
    u->_float[3] = v.x.w;
    u->_float[4] = v.y.x;
    u->_float[5] = v.y.y;
    u->_float[6] = v.y.z;
    u->_float[7] = v.y.w;
    u->_float[8] = v.z.x;
    u->_float[9] = v.z.y;
    u->_float[10] = v.z.z;
    u->_float[11] = v.z.w;
    u->_float[12] = v.w.x;
    u->_float[13] = v.w.y;
    u->_float[14] = v.w.z;
    u->_float[15] = v.w.w;
    uniforms[name] = u;
  }
}

GLProgram::GLProgram() {
  
  
  
  

}

GLStreamData::GLStreamData() {
  
  streams = std::shared_ptr<StreamMap >(new StreamMap());

}

uint32_t GLES20Pipeline::createRenderTarget(std::shared_ptr<RenderTarget> t_) {
  auto t = std::static_pointer_cast<data::RenderTarget>(t_);
  int32_t textureCount = 0;
  for (auto i_ : t->renderTargets) {
    auto i = std::static_pointer_cast<data::TargetItem>(i_);
    if (i->targetRef.valid && i->targetRef.data->tag == ::ImageRef::tag::TextureImage) {
      textureCount++;
    }
  }
  if (textureCount == 0) {
    return 0;
  }
  uint32_t fb;
  { unsigned int glObj; glGenFramebuffers(1, &glObj); fb = glObj; }
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  int32_t attachment = 0;
  int32_t textarget = 0;
  int32_t level = 0;
  uint32_t texture = 0;
  for (auto i_ : t->renderTargets) {
    auto i = std::static_pointer_cast<data::TargetItem>(i_);
    switch (i->targetSemantic->tag) {
      case ::ImageSemantic::tag::Color: attachment = GL_COLOR_ATTACHMENT0; break;
      case ::ImageSemantic::tag::Depth: attachment = GL_DEPTH_ATTACHMENT; break;
      case ::ImageSemantic::tag::Stencil: attachment = GL_STENCIL_ATTACHMENT; break;
    }
    if (i->targetRef.valid) {
      switch (i->targetRef.data->tag) {
        case ::ImageRef::tag::TextureImage: {
          auto ti = std::static_pointer_cast<data::TextureImage>(i->targetRef.data);
          texture = textures[ti->_0]->texture;
          textarget = GL_TEXTURE_2D;
          level = ti->_1;
          break;
        }
        case ::ImageRef::tag::Framebuffer: {
          texture = 0;
          textarget = GL_TEXTURE_2D;
          level = 0;
          break;
        }
      }
    } else {
      texture = 0;
      textarget = GL_TEXTURE_2D;
      level = 0;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textarget, texture, level);
  }
  return fb;
}

GLES20Pipeline::GLES20Pipeline(std::shared_ptr<Pipeline> ppl_) {
  
  input = std::shared_ptr<PipelineInput >(new PipelineInput());
  
  
  
  

  hasPipelineInput = false;
  screenTarget = 0;
  hasCurrentProgram = false;
  auto ppl = std::static_pointer_cast<data::Pipeline>(ppl_);
  pipeline = ppl;
  if (ppl->backend->tag != ::Backend::tag::WebGL1) {
    throw "unsupported backend";
  }
  for (auto i : ppl->textures) {
    textures.push_back(createTexture(i));
  }
  for (auto i : ppl->targets) {
    targets.push_back(createRenderTarget(i));
  }
  for (auto i : ppl->programs) {
    programs.push_back(createProgram(i));
  }
  for (auto i : ppl->streams) {
    streamData.push_back(createStreamData(i));
  }
  glReleaseShaderCompiler();
}

GLES20Pipeline::~GLES20Pipeline() {
  for (auto i : textures) {
    { unsigned int glObj = i->texture; glDeleteTextures(1, &glObj); }
  }
  for (auto i : targets) {
    { unsigned int glObj = i; glDeleteFramebuffers(1, &glObj); }
  }
  for (auto i : programs) {
    glDeleteProgram(i->program);
    glDeleteShader(i->vertexShader);
    glDeleteShader(i->fragmentShader);
  }
}

void GLES20Pipeline::setPipelineInput(std::shared_ptr<PipelineInput> i) {
  input = i;
  hasPipelineInput = true;
}

void GLES20Pipeline::render() {
  for (auto i : pipeline->commands) {
    switch (i->tag) {
      case ::Command::tag::SetRasterContext: {
        auto cmd = std::static_pointer_cast<data::SetRasterContext>(i);
        setupRasterContext(cmd->_0);
        break;
      }
      case ::Command::tag::SetAccumulationContext: {
        auto cmd = std::static_pointer_cast<data::SetAccumulationContext>(i);
        setupAccumulationContext(cmd->_0);
        break;
      }
      case ::Command::tag::SetTexture: {
        auto cmd = std::static_pointer_cast<data::SetTexture>(i);
        glActiveTexture(GL_TEXTURE0 + cmd->_0);
        glBindTexture(textures[cmd->_1]->target, textures[cmd->_1]->texture);
        break;
      }
      case ::Command::tag::SetProgram: {
        auto cmd = std::static_pointer_cast<data::SetProgram>(i);
        hasCurrentProgram = true;
        currentProgram = cmd->_0;
        glUseProgram(programs[currentProgram]->program);
        break;
      }
      case ::Command::tag::SetRenderTarget: {
        auto cmd = std::static_pointer_cast<data::SetRenderTarget>(i);
        uint32_t t = targets[cmd->_0];
        glBindFramebuffer(GL_FRAMEBUFFER, t == 0?screenTarget:t);
        if (hasPipelineInput) {
          glViewport(0, 0, input->screenWidth, input->screenHeight);
        }
        break;
      }
      case ::Command::tag::ClearRenderTarget: {
        auto cmd = std::static_pointer_cast<data::ClearRenderTarget>(i);
        uint32_t mask = 0;
        for (auto a : cmd->_0) {
          auto image = std::static_pointer_cast<data::ClearImage>(a);
          switch (image->imageSemantic->tag) {
            case ::ImageSemantic::tag::Depth: {
              auto v = std::static_pointer_cast<data::VFloat>(image->clearValue);
              glDepthMask(true);
              glClearDepthf(v->_0);
              mask |= GL_DEPTH_BUFFER_BIT;
              break;
            }
            case ::ImageSemantic::tag::Stencil: {
              auto v = std::static_pointer_cast<data::VWord>(image->clearValue);
              glClearStencil(v->_0);
              mask |= GL_STENCIL_BUFFER_BIT;
              break;
            }
            case ::ImageSemantic::tag::Color: {
              switch (image->clearValue->tag) {
                case ::Value::tag::VFloat: {
                  auto v = std::static_pointer_cast<data::VFloat>(image->clearValue);
                  glClearColor(v->_0, 0.0, 0.0, 1.0);
                  break;
                }
                case ::Value::tag::VV2F: {
                  auto v = std::static_pointer_cast<data::VV2F>(image->clearValue);
                  glClearColor(v->_0.x, v->_0.y, 0.0, 1.0);
                  break;
                }
                case ::Value::tag::VV3F: {
                  auto v = std::static_pointer_cast<data::VV3F>(image->clearValue);
                  glClearColor(v->_0.x, v->_0.y, v->_0.z, 1.0);
                  break;
                }
                case ::Value::tag::VV4F: {
                  auto v = std::static_pointer_cast<data::VV4F>(image->clearValue);
                  glClearColor(v->_0.x, v->_0.y, v->_0.z, v->_0.w);
                  break;
                }
                default:
                  glClearColor(0.0, 0.0, 0.0, 1.0);

              }
              glColorMask(true, true, true, true);
              mask |= GL_COLOR_BUFFER_BIT;
              break;
            }
          }
        }
        glClear(mask);
        break;
      }
      case ::Command::tag::SetSamplerUniform: {
        if (hasCurrentProgram) {
          auto cmd = std::static_pointer_cast<data::SetSamplerUniform>(i);
          int32_t sampler = programs[currentProgram]->programInTextures[cmd->_0];
          glUniform1i(sampler, cmd->_1);
        }
        break;
      }
      case ::Command::tag::RenderSlot: {
        if (hasPipelineInput && pipeline && hasCurrentProgram) {
          auto cmd = std::static_pointer_cast<data::RenderSlot>(i);
          auto slot = std::static_pointer_cast<data::Slot>(pipeline->slots[cmd->_0]);
          if (input->objectMap.count(slot->slotName)<=0) {
            break;
          }
          for (auto o : input->objectMap[slot->slotName]) {
            if (!o->enabled) {
              continue;
            }
            for (auto u : programs[currentProgram]->programUniforms) {
              if (o->uniforms.count(u.first)>0) {
                setUniformValue(u.second, o->uniforms[u.first]);
              } else {
                setUniformValue(u.second, input->uniforms[u.first]);
              }
            }
            for (auto s : programs[currentProgram]->programStreams) {
              setStream(s.second->index, o->streams->map[s.second->name]);
            }
            glDrawArrays(o->glMode, 0, o->glCount);
          }
        }
        break;
      }
      case ::Command::tag::RenderStream: {
        if (hasPipelineInput && pipeline && hasCurrentProgram) {
          auto cmd = std::static_pointer_cast<data::RenderStream>(i);
          std::shared_ptr<GLStreamData> data = streamData[cmd->_0];
          for (auto u : programs[currentProgram]->programUniforms) {
            setUniformValue(u.second, input->uniforms[u.first]);
          }
          for (auto s : programs[currentProgram]->programStreams) {
            setStream(s.second->index, data->streams->map[s.second->name]);
          }
          glDrawArrays(data->glMode, 0, data->glCount);
        }
        break;
      }
    }
  }
}

