//
// Created by Nicholas Newdigate on 10/02/2019.
//

#ifndef TEENSYAUDIOLIBRARY_RESAMPLINGLFSREADER_H
#define TEENSYAUDIOLIBRARY_RESAMPLINGLFSREADER_H

#include <cstdint>
#include "IndexableLittleFSFile.h"
#include "ResamplingReader.h"
#include "LittleFS.h"
#include "ResamplingSdReader.h"

namespace newdigate {

static constexpr size_t BUFFER_SIZE_LFS = 128;
static constexpr size_t BUFFER_COUNT_LFS = 2;

class ResamplingLfsReader : public ResamplingReader< IndexableLittleFSFile<BUFFER_SIZE_LFS, BUFFER_COUNT_LFS>, File > {
public:
    ResamplingLfsReader(LittleFS &fs) : 
        ResamplingReader(),
        _myFS(fs) 
    {
    }
    virtual ~ResamplingLfsReader() {
    }

    int16_t getSourceBufferValue(long index) override {
        return (*_sourceBuffer)[index];
    }

    int available(void)
    {
        return _playing;
    }

    File open(char *filename) override {
        return _myFS.open(filename);
    }

    void close(void) override
    {
        if (_playing)
            stop();
        if (_sourceBuffer != nullptr) {
            _sourceBuffer->close();
            delete _sourceBuffer;
            _sourceBuffer = nullptr;
        }
        if (_filename != nullptr) {
            delete [] _filename;
            _filename = nullptr;
        }
    }

    IndexableLittleFSFile<BUFFER_SIZE_LFS, BUFFER_COUNT_LFS>* createSourceBuffer() override {
        return new IndexableLittleFSFile<BUFFER_SIZE_LFS, BUFFER_COUNT_LFS>(_myFS, _filename);
    }

    uint32_t positionMillis(void) {
        if (_file_size == 0) return 0;
        if (!_useDualPlaybackHead) {
            return (uint32_t) (( (double)_bufferPosition1 * lengthMillis() ) / (double)(_file_size/2));
        } else 
        {
            if (_crossfade < 0.5)
                return (uint32_t) (( (double)_bufferPosition1 * lengthMillis() ) / (double)(_file_size/2));
            else
                return (uint32_t) (( (double)_bufferPosition2 * lengthMillis() ) / (double)(_file_size/2));
        }
    }

    uint32_t lengthMillis(void) {
        return ((uint64_t)_file_size * B2M) >> 32;
    }
    
protected:    
    LittleFS &_myFS;
};

}

#endif //TEENSYAUDIOLIBRARY_RESAMPLINGLFSREADER_H
