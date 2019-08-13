TARGET = sqlite3
CC = /opt/wasi-sdk/bin/clang
SYSROOT = /opt/wasi-sdk/share/sysroot
TARGET_TRIPLE = wasm32-unknown-wasi
CFLAGS = -nostartfiles -fvisibility=hidden
LDFLAGS = -Wl,--no-entry,--demangle,--allow-undefined
EXPORT_FUNCS = --export=allocate,--export=deallocate,--export=invoke,--export=load,--export=store
SQLITE_SRC = src/alter.c src/analyze.c src/attach.c src/auth.c src/backup.c src/bitvec.c src/btmutex.c src/btree.c src/build.c src/callback.c src/complete.c src/ctime.c src/date.c src/dbpage.c src/dbstat.c src/delete.c src/expr.c src/fault.c src/fkey.c src/fts3.c src/fts3_write.c src/fts3_aux.c src/fts3_expr.c src/fts3_hash.c src/fts3_icu.c src/fts3_porter.c src/fts3_snippet.c src/fts3_tokenize_vtab.c src/fts3_tokenizer.c src/fts3_tokenizer1.c src/fts3_unicode.c src/fts3_unicode2.c src/fts5.c src/func.c src/global.c src/hash.c src/insert.c src/json1.c src/legacy.c src/loadext.c src/main.c src/malloc.c src/memdb.c src/mem0.c src/mem1.c src/mem2.c src/memjournal.c src/notify.c src/opcodes.c src/os.c src/pager.c src/parse.c src/pcache.c src/pcache1.c src/pragma.c src/prepare.c src/printf.c src/random.c src/resolve.c src/rowset.c src/rtree.c src/select.c src/sqlite3session.c src/status.c src/stmt.c src/table.c src/tokenize.c src/treeview.c src/trigger.c src/update.c src/upsert.c src/userauth.c src/utf.c src/util.c src/vacuum.c src/vdbe.c src/vdbeapi.c src/vdbeaux.c src/vdbeblob.c src/vdbemem.c src/vdbesort.c src/vdbetrace.c src/vtab.c src/wal.c src/walker.c src/where.c src/wherecode.c src/whereexpr.c src/window.c
WRAPPER_SRC = src/wrapper.c
SQLITE_FLAGS = -DSQLITE_CORE -D_HAVE_SQLITE_CONFIG_H -DBUILD_sqlite -DNDEBUG -DSQLITE_THREADSAFE=0 -DHAVE_READLINE=0 -DHAVE_EDITLINE=0 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_ENABLE_JSON1 -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_RTREE -DSQLITE_ENABLE_EXPLAIN_COMMENTS -DSQLITE_ENABLE_UNKNOWN_SQL_FUNCTION -DSQLITE_ENABLE_STMTVTAB -DSQLITE_ENABLE_DBPAGE_VTAB -DSQLITE_ENABLE_DBSTAT_VTAB -DSQLITE_ENABLE_OFFSET_SQL_FUNC -DSQLITE_ENABLE_DESERIALIZE -DSQLITE_INTROSPECTION_PRAGMAS -DSQLITE_OMIT_POPEN
SDK = sdk/logger.c

.PHONY: default all clean

default: $(TARGET)
all: default

$(TARGET): $(SDK) $(SQLITE_SRC) $(WRAPPER_SRC)
	$(CC) --sysroot=$(SYSROOT) --target=$(TARGET_TRIPLE) -O2 $(SQLITE_FLAGS) $(CFLAGS) $(LDFLAGS) -Wl,$(EXPORT_FUNCS) $^ -o $@.wasm

.PRECIOUS: $(TARGET)

clean:
	-rm -f $(TARGET).wasm
