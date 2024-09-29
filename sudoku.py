import numpy as np

class Grid:
	def __init__(self):
		self.boxSize = 0
		self.gridSize = 0
		self.grid = None

	def init(self, box_size, grid_size):
		self.boxSize = box_size
		self.gridSize = grid_size
		self.grid = np.zeros((grid_size, grid_size), dtype=np.int32)

	# 求行集合
	def getRowSet(self, row_index):
		row_nums = set()
		for i in range(self.gridSize):
			row_nums.add(self.grid[row_index][i])
		return row_nums

	# 求列集合
	def getColSet(self, col_index):
		col_nums = set()
		for i in range(self.gridSize):
			col_nums.add(self.grid[i][col_index])
		return col_nums

	# 求九宫格集合
	def getBoxSet(self, row_index, col_index):
		box_nums = set()
		boxStartRow = row_index - row_index % self.boxSize;
		boxStartCol = col_index - col_index % self.boxSize;
		for i in range(self.boxSize):
			for j in range(self.boxSize):
				box_nums.add(self.grid[boxStartRow + i][boxStartCol + j])

		return box_nums

	# 输出网格
	def printGrid(self):
		if self.grid is None:
			print("printGrid failed, grid is none")
			return
		print("Grid:")
		for i in range(self.gridSize):
			for j in range(self.gridSize):
				print(self.grid[i][j], end=' ')
			print("")

class Sudoku(Grid):

	def __init__(self):
		super().__init__()
		self.candidate = []

	def parse(self, input):
		if len(input) != 9 * 9:
			print("Parse failed, input length is not a sudoku size")
			return 
		self.init(3, 9)
		for i in range(self.gridSize):
			for j in range(self.gridSize):
				self.grid[i][j] = int(input[i * self.gridSize + j])

	# 推理候选值
	def getInference(self):
		if self.grid is None:
			print("Inference failed, grid is none")
			return

		full_digits = set((1, 2, 3, 4, 5, 6, 7, 8, 9))

		for i in range(self.gridSize):
			for j in range(self.gridSize):
				if self.grid[i][j] != 0:
					self.candidate.append(set([self.grid[i][j]]))
					continue

				# 当前位置行、列、九宫格集合
				row_nums = self.getRowSet(i)
				col_nums = self.getColSet(j)
				box_nums = self.getBoxSet(i, j)
				exist_nums = set.union(row_nums, col_nums, box_nums)

				# 求差集，得到候选值集合
				candidate_nums = full_digits.difference(exist_nums)
				self.candidate.append(candidate_nums)

		return self.candidate

	# 输出候选值
	def printCandidate(self):
		if self.grid is None:
			print("printCandidate failed, grid is none")
			return
		print("Sudoku candidate:")
		for i in range(self.gridSize):
			for j in range(self.gridSize):
				print("row, col({}, {}):".format(i, j), self.candidate[i * self.gridSize + j])

	# 测试候选值
	def testCandidate(self):
		if self.grid is None:
			print("testCandidate failed, grid is none")
			return
		print("Sudoku test:")
		for i in range(self.gridSize):
			for j in range(self.gridSize):
				print("row, col({}, {}): ".format(i, j), end="")
				if self.grid[i][j] != 0:
					print("accept")
					continue
				flag = 1
				row_nums = self.getRowSet(i)
				col_nums = self.getColSet(j)
				box_nums = self.getBoxSet(i, j)
				for each_candidate in self.candidate[i * self.gridSize + j]:
					if each_candidate in row_nums:
						print("wrong because {} in row".format(each_candidate))
						flag = 0
						break
					if each_candidate in col_nums:
						print("wrong because {} in col".format(each_candidate))
						flag = 0
						break
					if each_candidate in box_nums:
						print("wrong because {} in box".format(each_candidate))
						flag = 0
						break
				if flag:
					print("accept")


sudoku = Sudoku()
# s = "017903600000080000900000507072010430000402070064370250701000065000030000005601720"
s = input()
sudoku.parse(s) # 扫描输入
sudoku.getInference() # 推理候选值
sudoku.printGrid() # 输出网格
sudoku.printCandidate() # 输出候选值
sudoku.testCandidate() # 测试候选值是否正确