/*!
 * @file sequence.hpp
 *
 * @brief Sequence class header file
 */

#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>

namespace bioparser {
    template<class T>
    class FastaParser;

    template<class T>
    class FastqParser;
}

namespace racon {

class Sequence;
std::unique_ptr<Sequence> createSequence(const std::string& name,
    const std::string& data);
std::unique_ptr<Sequence> createSequence(const std::string& name,
    const std::string& data, const std::string& cigar, const int32_t id);

class Sequence {
public:
    ~Sequence() = default;

    const std::string& name() const {
        return name_;
    }

    const std::string& data() const {
        return data_;
    }

    const std::string& reverse_complement() const {
        return reverse_complement_;
    }

    const std::string& quality() const {
        return quality_;
    }

    const std::string& reverse_quality() const {
        return reverse_quality_;
    }

    const std::string& cigar() const {
        return cigar_;
    }

    int32_t id() const {
        return id_;
    }

    void id(int32_t val) {
        id_ = val;
    }

    void create_reverse_complement();

    void transmute(bool has_name, bool has_data, bool has_reverse_data);

    friend bioparser::FastaParser<Sequence>;
    friend bioparser::FastqParser<Sequence>;
    friend std::unique_ptr<Sequence> createSequence(const std::string& name,
        const std::string& data);
    friend std::unique_ptr<Sequence> createSequence(const std::string& name,
        const std::string& data, const std::string& cigar, const int32_t id);

private:
    Sequence(const char* name, uint32_t name_length, const char* data,
        uint32_t data_length);
    Sequence(const char* name, uint32_t name_length, const char* data,
        uint32_t data_length, const char* quality, uint32_t quality_length);
    Sequence(const std::string& name, const std::string& data);
    Sequence(const std::string& name, const std::string& data, const std::string& cigar,
        const int32_t id);
    Sequence(const Sequence&) = delete;
    const Sequence& operator=(const Sequence&) = delete;

    std::string name_;
    std::string data_;
    std::string reverse_complement_;
    std::string quality_;
    std::string reverse_quality_;
    std::string cigar_;
    int32_t id_;
};

}
