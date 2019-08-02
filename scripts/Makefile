.PHONY: clean
clean:
	@echo "cleaning .o files in build and binary files in bin."
	@for file in ../*/build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ../build/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@for file in ../*/bin/*; do rm -rf $$file; echo Delete binary file: $$file; done
	@for file in ../*/*/bin/*; do rm -rf $$file; echo Delete Object file: $$file; done
	@echo "finished"

