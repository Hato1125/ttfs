#ifndef _TTFS_TTFS_HH
#define _TTFS_TTFS_HH

#include <print>
#include <vector>
#include <variant>
#include <cstdint>
#include <algorithm>

#define __TTFS_PANIC(message) do {  \
  std::println(stderr, message);    \
  std::abort();                     \
} while (0)

namespace ttfs {
  constexpr std::uint8_t major = 1u;
  constexpr std::uint8_t minor = 0u;
  constexpr std::uint8_t patch = 0u;

  enum class note_type : std::uint8_t {
    rest,
    don,
    katsu,
    big_don,
    big_katsu,
  };

  enum class event_type : std::uint8_t {
    rest,
    measure,
    gogo_begin,
    gogo_end,
  };

  enum class course_type : std::uint8_t {
    easy,
    normal,
    hard,
    oni,
    edit,
  };

  class note {
  public:
    note_type type = note_type::rest;
    float bpm = 0.0f;
    float time = 0.0f;
    float speed = 0.0f;

    auto operator<=>(const note&) const noexcept = default;
  };

  class event {
  public:
    event_type type = event_type::rest;
    float time = 0.0f;
    float speed = 0.0f;

    auto operator<=>(const event&) const noexcept = default;
  };

  class section {
  public:
    std::vector<note> notes;
    std::vector<event> events;

    std::size_t don_size() const noexcept {
      return std::count_if(notes.begin(), notes.end(), [](const note& note) {
        return note.type == note_type::don
          || note.type == note_type::big_don;
      });
    }

    std::size_t katsu_size() const noexcept {
      return std::count_if(notes.begin(), notes.end(), [](const note& note) {
        return note.type == note_type::katsu
          || note.type == note_type::big_katsu;
      });
    }

    std::size_t notes_size() const noexcept { return notes.size(); }
    std::size_t events_size() const noexcept { return events.size(); }

    auto operator<=>(const section&) const noexcept = default;
  };

  class course {
    struct doubles {
      section p1;
      section p2;
    };

  public:
    constexpr course(
      course_type type,
      std::uint8_t level,
      const section& section,
      std::uint32_t score_init = 0,
      std::uint32_t score_diff = 0
    ) noexcept : _type(type),
      _level(level),
      _score_init(score_init),
      _score_diff(score_diff),
      _sections(section) {}

    constexpr course(
      course_type type,
      std::uint8_t level,
      const doubles& doubles,
      std::uint32_t score_init = 0,
      std::uint32_t score_diff = 0
    ) noexcept : _type(type),
      _level(level),
      _score_init(score_init),
      _score_diff(score_diff),
      _sections(doubles) {}

    const section& p1_section() const noexcept {
      return has_doubles()
        ? std::get<doubles>(_sections).p1
        : std::get<section>(_sections);
    }

    const section& p2_section() const noexcept {
      if (!has_doubles()) {
        __TTFS_PANIC("no doubles section for p2_section()");
      }

      return std::get<doubles>(_sections).p2;
    }

    bool has_doubles() const noexcept {
      return std::holds_alternative<doubles>(_sections);
    }

    course_type type() const noexcept { return _type; }
    std::uint8_t level() const noexcept { return _level; }
    std::uint32_t score_init() const noexcept { return _score_init; }
    std::uint32_t score_diff() const noexcept { return _score_diff; }

  private:
    course_type _type;
    std::uint8_t _level;
    std::uint32_t _score_init;
    std::uint32_t _score_diff;
    std::variant<section, doubles> _sections;
  };
}

#endif
