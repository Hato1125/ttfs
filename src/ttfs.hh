#ifndef _TTFS_TTFS_HH
#define _TTFS_TTFS_HH

#include <map>
#include <string>
#include <string_view>
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
  constexpr std::uint8_t major = 1;
  constexpr std::uint8_t minor = 0;
  constexpr std::uint8_t patch = 0;

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

  enum class genre_type : std::uint8_t {
    unknown,
    pop,
    kids,
    namco,
    classic,
    variety,
    game,
    vocaloid,
    anime,
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
    constexpr course() noexcept : _type(course_type::easy),
      _level(0),
      _score_init(0),
      _score_diff(0),
      _sections({}) {}

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

  struct chart_info {
    genre_type genre;

    std::string_view wave;
    std::string_view title;
    std::string_view subtitle;
    float bpm;
    float offset;
    float demostart;

    std::optional<course> easy;
    std::optional<course> normal;
    std::optional<course> hard;
    std::optional<course> oni;
    std::optional<course> edit;

    std::map<
      std::string,
      std::variant<std::string, std::int32_t>
    > headers;
  };


  class chart {
  public:
    constexpr chart() noexcept : _info{
      .genre = genre_type::unknown,
      .bpm = 0.0f,
      .offset = 0.0f,
      .demostart = 0.0f,
    } {}

    constexpr chart(chart_info&& info) noexcept : _info(std::move(info)) {}

    bool has_easy() const noexcept { return _info.easy.has_value(); }
    bool has_normal() const noexcept { return _info.normal.has_value(); }
    bool has_hard() const noexcept { return _info.hard.has_value(); }
    bool has_oni() const noexcept { return _info.oni.has_value(); }
    bool has_edit() const noexcept { return _info.edit.has_value(); }

    const course& easy() const noexcept {
      if (!has_easy()) {
        __TTFS_PANIC("no easy section for easy()");
      }
      return *_info.easy;
    }

    const course& normal() const noexcept {
      if (!has_normal()) {
        __TTFS_PANIC("no normal section for normal()");
      }
      return *_info.normal;
    }

    const course& hard() const noexcept {
      if (!has_hard()) {
        __TTFS_PANIC("no hard section for hard()");
      }
      return *_info.hard;
    }

    const course& oni() const noexcept {
      if (!has_oni()) {
        __TTFS_PANIC("no oni section for oni()");
      }
      return *_info.oni;
    }


    const course& edit() const noexcept {
      if (!has_edit()) {
        __TTFS_PANIC("no edit section for edit()");
      }
      return *_info.edit;
    }

    genre_type genre() const noexcept { return _info.genre; }

    float bpm() const noexcept { return _info.bpm; }
    float offset() const noexcept { return _info.offset; }
    float demostart() const noexcept { return _info.demostart; }
    std::string_view wave() const noexcept { return _info.wave; }
    std::string_view title() const noexcept { return _info.title; }
    std::string_view subtitle() const noexcept { return _info.subtitle; }

    template <typename T>
    std::optional<std::reference_wrapper<const T>> get(
      const std::string& name
    ) const noexcept {
      const auto it = _info.headers.find(name);

      if (it != _info.headers.end()) {
        if (const auto* ptr = std::get_if<T>(&it->second)) {
          return std::ref(*ptr);
        }
      }

      return std::nullopt;
    }

  private:
    chart_info _info;
  };
}

#endif
