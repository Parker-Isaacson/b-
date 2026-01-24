#include <cstdint>
#include <cassert>
#include <array>

// This was the initial attempt at packing, though it will be dropped for know as its not known if it will provide performance increases.

// enum class Rank : std::uint8_t { }; // This is not real, they are already numbers 1-8

enum class File : std::uint8_t { a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6, h = 7, };

struct Square {
    std::uint8_t sq = 0;

    constexpr Square() = default;

    constexpr Square(std::uint8_t rank, File file) { set_rank(rank); set_file(file); }

    constexpr std::uint8_t get_rank() const noexcept { return static_cast<std::uint8_t>(sq & 0x0f) + 1; }
    constexpr File         get_file() const noexcept { return static_cast<File>((sq >> 4) & 0x0f); }

    constexpr void set_rank(std::uint8_t rank) noexcept {
        assert(0 < rank && rank < 9);
        rank -= 1;
        sq = static_cast<std::uint8_t>((sq & 0xf0) | (rank & 0x0f));
    }

    constexpr void set_file(File file) noexcept {
        std::uint8_t f = static_cast<std::uint8_t>(file);
        assert(f < 8);
        sq = static_cast<std::uint8_t>((sq & 0x0f) | ((f & 0x0f) << 4));
    }

    static constexpr Square from_packed(std::uint8_t packed) noexcept {
        Square s;
        s.sq = packed;
        return s;
    }
};

struct Move {
    std::uint16_t move = 0;

    constexpr Move() = default;

    constexpr Move(Square from, Square to) { set_from(from); set_to(to); }

    constexpr Move(std::uint8_t from_rank, File from_file, std::uint8_t to_rank, File to_file)
        : Move(Square(from_rank, from_file), Square(to_rank, to_file)) {}

    constexpr Square get_from() const noexcept { return Square::from_packed(static_cast<std::uint8_t>(move & 0x00ff)); }
    constexpr Square get_to()   const noexcept { return Square::from_packed(static_cast<std::uint8_t>((move >> 8) & 0x00ff)); }

    constexpr void set_from(Square from) noexcept {
        move = static_cast<std::uint16_t>((move & 0xff00) | static_cast<std::uint16_t>(from.sq));
    }

    constexpr void set_to(Square to) noexcept {
        move = static_cast<std::uint16_t>((move & 0x00ff) | (static_cast<std::uint16_t>(to.sq) << 8));
    }
};

static_assert(sizeof(Square) == 1);
static_assert(sizeof(Move)   == 2);

enum class Piece : std::uint8_t {
    Empty        = 0b0000,

    White_King   = 0b0001,
    White_Queen  = 0b0010,
    White_Bishop = 0b0011,
    White_Knight = 0b0100,
    White_Rook   = 0b0101,
    White_Pawn   = 0b0110,

    Black_King   = 0b1001,
    Black_Queen  = 0b1010,
    Black_Bishop = 0b1011,
    Black_Knight = 0b1100,
    Black_Rook   = 0b1101,
    Black_Pawn   = 0b1110,
};

using Board = std::array<std::array<Piece, 8>, 8>;

class Game {
    private:
        Board board{};

    public:
        Game() {}
};
