//
// Created by Nicholas Newdigate on 10/02/2019.
//

#ifndef TEENSYAUDIOLIBRARY_RESAMPLINGSERIALFLASHREADER_H
#define TEENSYAUDIOLIBRARY_RESAMPLINGSERIALFLASHREADER_H

#include <cstdint>
#include "spi_interrupt.h"
#include "loop_type.h"
#include "interpolation.h"
#include "IndexableSerialFlashFile.h"
#include "ResamplingReader.h"
#include "SerialFlash.h"

namespace newdigate {

static constexpr size_t BUFFER_SIZE_SERIALFLASH = 128;
static constexpr size_t BUFFER_COUNT_SERIALFLASH = 2;

class ResamplingSerialFlashReader : public ResamplingReader< IndexableSerialFlashFile<BUFFER_SIZE_SERIALFLASH, BUFFER_COUNT_SERIALFLASH>, SerialFlashFile > {
public:
    ResamplingSerialFlashReader(SerialFlashChip &fs) : 
        ResamplingReader(),
        _myFS(fs) 
    {
    }

    virtual ~ResamplingSerialFlashReader() {
    }

    int16_t getSourceBufferValue(long index) override {
        return (*_sourceBuffer)[index];
    }

    int available(void)
    {
        return _playing;
    }

    SerialFlashFile open(char *filename) override {
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

    IndexableSerialFlashFile<BUFFER_SIZE_SERIALFLASH, BUFFER_COUNT_SERIALFLASH>* createSourceBuffer() override {
        return new IndexableSerialFlashFile<BUFFER_SIZE_SERIALFLASH, BUFFER_COUNT_SERIALFLASH>(_myFS, _filename);
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
    SerialFlashChip &_myFS;
};

}

#endif //TEENSYAUDIOLIBRARY_RESAMPLINGSERIALFLASHREADER_H
