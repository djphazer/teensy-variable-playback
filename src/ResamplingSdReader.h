//
// Created by Nicholas Newdigate on 10/02/2019.
//

#ifndef TEENSYAUDIOLIBRARY_RESAMPLINGSDREADER_H
#define TEENSYAUDIOLIBRARY_RESAMPLINGSDREADER_H

#include "SD.h"
#include <cstdint>
#include "spi_interrupt.h"
#include "loop_type.h"
#include "interpolation.h"
#include "IndexableSDFile.h"
#include "ResamplingReader.h"

namespace newdigate {

static constexpr size_t BUFFER_SIZE_SD = 128;
static constexpr size_t BUFFER_COUNT_SD = 4;

class ResamplingSdReader : public ResamplingReader< IndexableSDFile<BUFFER_SIZE_SD, BUFFER_COUNT_SD>, File > {
public:
    ResamplingSdReader(SDClass &sd = SD) : 
        ResamplingReader(),
        _sd(sd)
    {
    }
    
    virtual ~ResamplingSdReader() {
    }

    int16_t getSourceBufferValue(long index) override {
        return (_sourceBuffer == nullptr)
					?0
					:(*_sourceBuffer)[index];
    }

    int available(void)
    {
        return _playing;
    }

    File open(char *filename) override {
        return _sd.open(filename);
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

    IndexableSDFile<BUFFER_SIZE_SD, BUFFER_COUNT_SD>* createSourceBuffer() override {
        File f = open(_filename);
        return new IndexableSDFile<BUFFER_SIZE_SD, BUFFER_COUNT_SD>(_filename, _sd, f);
    }

    IndexableSDFile<BUFFER_SIZE_SD, BUFFER_COUNT_SD>* createSourceBuffer(File& file) override {
        return new IndexableSDFile<BUFFER_SIZE_SD, BUFFER_COUNT_SD>(_filename, _sd, file);
    }

protected:    
     SDClass &_sd;
};

}

#endif //TEENSYAUDIOLIBRARY_RESAMPLINGSDREADER_H
