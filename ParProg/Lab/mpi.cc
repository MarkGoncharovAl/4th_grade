#include "helper.h"

constexpr int ISIZE = 1800;
constexpr int JSIZE = 1800;
constexpr int SIZE = ISIZE * JSIZE;
constexpr double error = 0.01;

struct data_t {
  data_t(int size) { data.resize(size); }
  double *operator[](int x) noexcept { return data.data() + x * ISIZE; }
  std::vector<double> data;
};

int size = 0, rank = 0;

void sequential1G1(data_t &data) {
  for (int i = 0; i < ISIZE; ++i)
    for (int j = 0; j < JSIZE; ++j)
      data[i][j] = 10 * i + j;
}
void sequential1G2 (data_t & data) {
  for (int i = 1; i < ISIZE; ++i)
    for (int j = 3; j < JSIZE; ++j)
      data[i][j] = std::sin(2 * data[i - 1][j - 3]);
}

void parallel1G1(data_t &data) {
  auto curSize = ISIZE / (size - 1);

  if (rank != mpi::rank_main) {
    for (int i = (rank - 1) * curSize; i < rank * curSize; ++i) {
      for (int j = 0; j < JSIZE; ++j)
        data[i][j] = 10 * i + j;
      act(MPI_Send(data[i], JSIZE, MPI_DOUBLE, /*dest*/ 0, 0, MPI_COMM_WORLD));
    }
  } else { /*rank == 0*/
    for (int i = 0; i < curSize; ++i)
      for (int j = 1; j < size; ++j)
        act(MPI_Recv(data[i + (j - 1) * curSize], JSIZE, MPI_DOUBLE, j, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE));
  }

  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  MPI_Bcast(data[0], SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
void parallel1G2 (data_t& data) {
  int cur2Size = (JSIZE - 3) / (size - 1);
  for (int i = 1; i < ISIZE; ++i) {
    if (rank != 0) {
      auto start_elem = 3 + (rank - 1) * cur2Size;
      for (int j = start_elem; j < start_elem + cur2Size; ++j)
        data[i][j] = std::sin(2 * data[i - 1][j - 3]);
      act(MPI_Send(&(data[i][start_elem]), cur2Size, MPI_DOUBLE, /*dest*/ 0, 0,
                   MPI_COMM_WORLD));
    } else /*rank == 0*/
    {
      for (int j = 1; j < size; ++j)
        act(MPI_Recv(&(data[i][3 + (j - 1) * cur2Size]), cur2Size, MPI_DOUBLE,
                     j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE));
    }
    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    MPI_Bcast(data[i], JSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
}

bool compare(data_t &lhs, data_t &rhs) {
  for (int i = 0; i < ISIZE; ++i)
    for (int j = 0; j < JSIZE; ++j)
      if (std::abs(lhs[i][j] - rhs[i][j]) > error) {
        std::cerr << i << " : " << j << "\n";
        std::cerr << lhs[i][j] << " : " << rhs[i][j] << "\n";
        return false;
      }
  return true;
}

int main() {
  auto [sizeTmp, rankTmp] = mpi::init();
  size = sizeTmp;
  rank = rankTmp;

  data_t data(SIZE);
  if (rank == 0) {
    std::cout << "\n";
    data.data = Rand{}.getVector<double>(SIZE);
  }

  MPI_Bcast(data[0], SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Test
  auto copy1 = data;
  auto copy2 = data;

  if (rank == mpi::rank_main) {
    sequential1G1(copy1);
    sequential1G2 (copy1);
  }
  parallel1G1(copy2);
  parallel1G2(copy2);
  if (rank == mpi::rank_main)
    if (compare(copy1, copy2))
      std::cout << "Algorithm works fine!\n";
    else
      std::cerr << "Matrixes aren't equal!\n";

  // Benchmark
  constexpr int sizeTest = 20;
  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  auto startSeq = MPI_Wtime();

  if (rank == mpi::rank_main)
    for (int i = 0; i < sizeTest; ++i)
      sequential1G1(data);

  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  auto startSeq2 = MPI_Wtime ();

  if (rank == mpi::rank_main)
    for (int i = 0; i < sizeTest; ++i)
      sequential1G2 (data);

  MPI_Barrier (MPI_COMM_WORLD); /* IMPORTANT */
  auto endSeq = MPI_Wtime();

  for (int i = 0; i < sizeTest; ++i)
    parallel1G1(data);

  MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
  auto endPar1 = MPI_Wtime();

  for (int i = 0; i < sizeTest; ++i)
    parallel1G2 (data);

  MPI_Barrier (MPI_COMM_WORLD); /* IMPORTANT */
  auto endPar2 = MPI_Wtime ();

  if (rank == mpi::rank_main) {
    std::cout << "Time sequential1: " << (startSeq2 - startSeq) / sizeTest << "\n";
    std::cout << "Time sequential2: " << (endSeq - startSeq2) / sizeTest << "\n";
    std::cout << "Time parallel1: " << (endPar1 - endSeq) / sizeTest << "\n";
    std::cout << "Time parallel2: " << (endPar2 - endPar1) / sizeTest << "\n";
  }

  quit();
}