all:
	@for d in `find src -type d`; do \
		if [ -f "$$d/CMakeLists.txt" ]; then \
			echo "[Test] $$d"; \
			make -C $$d || exit 1; \
		fi; \
	done

clean:
	@for d in `find src -type d`; do \
		if [ -f "$$d/CMakeLists.txt" ]; then \
			make -C $$d clean; \
		fi; \
	done
