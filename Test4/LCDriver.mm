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

@interface LCDriver () {
    GLES20Pipeline * _ppl;
    std::shared_ptr<PipelineInput> _input;
    std::shared_ptr<Buffer> _buffer;
    std::shared_ptr<StreamMap> _attributes;
    std::shared_ptr<Object> _object;
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

    NSString *filePath = [[NSBundle mainBundle] pathForResource:@"gfx02" ofType:@"json"];
    NSData *myData = [NSData dataWithContentsOfFile:filePath];
    if (myData) {
        json jobjIn = json::parse((const char*)myData.bytes);
        auto ppl = fromJSON(W<std::shared_ptr<Pipeline>>(),jobjIn);
        _ppl = new GLES20Pipeline(ppl);
        _ppl->screenTarget = screenTarget;

        _input = std::shared_ptr<PipelineInput>(new PipelineInput());
        _input->setScreenSize(w, h);
        _ppl->setPipelineInput(_input);

        // input setup
        _buffer = std::shared_ptr<Buffer>(new Buffer());
        _attributes = std::shared_ptr<StreamMap>(new StreamMap());
        _attributes->add("position4",  Type::FLOAT_VEC4, _buffer, _buffer->add(g_vertex_buffer_data));
        _attributes->add("vertexUV",   Type::FLOAT_VEC2, _buffer, _buffer->add(g_uv_buffer_data));
        _buffer->freeze();
        _attributes->validate();

        // add game 3D object
        _object = _input->createObject("stream4", Primitive::TriangleList, _attributes, {});
        //M44F mat;
        //_object->setUniform("ModelMatrix", mat);

    }
}

- (void)render {
    _ppl->render();
}
@end
