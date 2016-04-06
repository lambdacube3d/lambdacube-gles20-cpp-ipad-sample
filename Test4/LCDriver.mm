//
//  LCDriver.m
//  Test4
//
//  Created by Csaba Hruska on 02/10/15.
//  Copyright © 2015 Csaba Hruska. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "LCDriver.h"
#include "LambdaCube.hpp"
#include "IR.hpp"
#include <OpenGLES/ES2/gl.h>


@interface LCDriver () {
    GLES20Pipeline * _ppl;
    GLES20Pipeline * _ppl2;
    std::shared_ptr<PipelineInput> _input;
    std::shared_ptr<GLBuffer> _buffer;
    std::shared_ptr<StreamMap> _attributes;
    std::shared_ptr<GLObject> _object;
}
@end

@implementation LCDriver

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

std::vector<float> g_vertex_buffer_data =
    {   1.0 ,   1.0 , (-1.0), 1.0
    ,   1.0 , (-1.0), (-1.0), 1.0
    , (-1.0), (-1.0), (-1.0), 1.0
    ,   1.0 ,   1.0 , (-1.0), 1.0
    , (-1.0), (-1.0), (-1.0), 1.0
    , (-1.0),   1.0 , (-1.0), 1.0
    ,   1.0 ,   1.0 , (-1.0), 1.0
    ,   1.0 ,   1.0 ,   1.0 , 1.0
    ,   1.0 , (-1.0),   1.0 , 1.0
    ,   1.0 ,   1.0 , (-1.0), 1.0
    ,   1.0 , (-1.0),   1.0 , 1.0
    ,   1.0 , (-1.0), (-1.0), 1.0
    ,   1.0 ,   1.0 ,   1.0 , 1.0
    , (-1.0), (-1.0),   1.0 , 1.0
    ,   1.0 , (-1.0),   1.0 , 1.0
    ,   1.0 ,   1.0 ,   1.0 , 1.0
    , (-1.0),   1.0 ,   1.0 , 1.0
    , (-1.0), (-1.0),   1.0 , 1.0
    , (-1.0),   1.0 ,   1.0 , 1.0
    , (-1.0), (-1.0), (-1.0), 1.0
    , (-1.0), (-1.0),   1.0 , 1.0
    , (-1.0),   1.0 ,   1.0 , 1.0
    , (-1.0),   1.0 , (-1.0), 1.0
    , (-1.0), (-1.0), (-1.0), 1.0
    ,   1.0 ,   1.0 , (-1.0), 1.0
    , (-1.0),   1.0 , (-1.0), 1.0
    , (-1.0),   1.0 ,   1.0 , 1.0
    ,   1.0 ,   1.0 , (-1.0), 1.0
    , (-1.0),   1.0 ,   1.0 , 1.0
    ,   1.0 ,   1.0 ,   1.0 , 1.0
    ,   1.0 ,   1.0 , (-1.0), 1.0
    ,   1.0 ,   1.0 ,   1.0 , 1.0
    , (-1.0),   1.0 ,   1.0 , 1.0
    ,   1.0 ,   1.0 , (-1.0), 1.0
    , (-1.0),   1.0 ,   1.0 , 1.0
    , (-1.0),   1.0 , (-1.0), 1.0
    };

//  Two UV coordinatesfor each vertex. They were created with Blender.
std::vector<float> g_uv_buffer_data =
    { 0.0, 0.0
    , 0.0, 1.0
    , 1.0, 1.0
    , 0.0, 0.0
    , 1.0, 1.0
    , 1.0, 0.0
    , 0.0, 0.0
    , 1.0, 0.0
    , 1.0, 1.0
    , 0.0, 0.0
    , 1.0, 1.0
    , 0.0, 1.0
    , 1.0, 0.0
    , 0.0, 1.0
    , 1.0, 1.0
    , 1.0, 0.0
    , 0.0, 0.0
    , 0.0, 1.0
    , 0.0, 0.0
    , 1.0, 1.0
    , 0.0, 1.0
    , 0.0, 0.0
    , 1.0, 0.0
    , 1.0, 1.0
    , 0.0, 0.0
    , 1.0, 0.0
    , 1.0, 1.0
    , 0.0, 0.0
    , 1.0, 1.0
    , 0.0, 1.0
    , 0.0, 0.0
    , 0.0, 1.0
    , 1.0, 1.0
    , 0.0, 0.0
    , 1.0, 1.0
    , 1.0, 0.0
    };

- (void)sayHello:(unsigned int)screenTarget width:(unsigned int)w height:(unsigned int) h {

    // pipeline 1
    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"rotate01" ofType:@"json"];
    NSData *myData = [NSData dataWithContentsOfFile:filePath];
    if (!myData) return;
    json jobjIn = json::parse((const char*)myData.bytes);
    auto ppl = fromJSON(W<std::shared_ptr<Pipeline>>(),jobjIn);
    _ppl = new GLES20Pipeline(ppl);
    _ppl->screenTarget = screenTarget;

    // pipeline 2
    filePath = [[NSBundle mainBundle] pathForResource:@"Tetrahedron" ofType:@"json"];
    myData = [NSData dataWithContentsOfFile:filePath];
    if (!myData) return;
    jobjIn = json::parse((const char*)myData.bytes);
    auto ppl2 = fromJSON(W<std::shared_ptr<Pipeline>>(),jobjIn);
    _ppl2 = new GLES20Pipeline(ppl2);
    _ppl2->screenTarget = screenTarget;

    // pipeline input
    _input = std::shared_ptr<PipelineInput>(new PipelineInput());
    _input->setScreenSize(w, h);
    _ppl->setPipelineInput(_input);
    _ppl2->setPipelineInput(_input);

    // input setup
    _buffer = std::shared_ptr<GLBuffer>(new GLBuffer());
    _attributes = std::shared_ptr<StreamMap>(new StreamMap());
    _attributes->add("position4",  Type::FLOAT_VEC4, _buffer, _buffer->add(g_vertex_buffer_data.data(),GL_FLOAT,g_vertex_buffer_data.size()));
    _attributes->add("vertexUV",   Type::FLOAT_VEC2, _buffer, _buffer->add(g_uv_buffer_data.data(),GL_FLOAT, g_uv_buffer_data.size()));
    _buffer->freeze();
    _attributes->validate();

    // add game 3D object
    _object = _input->createObject("stream4", Primitive::TriangleList, _attributes, {});
    M44F mat = { {1.0, 0.0, 0.0, 0.0}
               , {0.0, 1.0, 0.0, 0.0}
               , {0.0, 0.0, 1.0, 0.0}
               , {0.0, 0.0, 0.0, 1.0}
               };
    _object->setUniform("MVP", mat);
    float time = 0.0;
    _object->setUniform("Time", time);
}

- (void)render:(float)t {
    if(_object && _ppl) {
        _input->setUniform("Time", t);
        _object->setUniform("Time", t);
     _ppl->render();
    }
}

- (void)render2:(float)t {
    if(_object && _ppl2) {
     _object->setUniform("Time", t);
        _input->setUniform("Time", t);

     _ppl2->render();

    }
}
@end
