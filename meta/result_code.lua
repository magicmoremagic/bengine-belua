include 'common/enum'
include 'common/load_tsv'

local data = load_tsv(resolve_include_path('result_code.tsv'))

result_code = make_enum_class('be::belua::ResultCode', 'U8', data)

local errc_map = {
   name = 'generic_error_condition',
   enum = result_code,
   type = 'int',
   no_cast = true,
   mapper = function (constant)
      if constant.errc and #constant.errc > 0 then
         return 'static_cast<int>(std::errc::' .. constant.errc .. ')'
      else
         return '0'
      end
   end,
   default = '0'
}

local msg_map = {
   name = 'result_code_msg',
   enum = result_code,
   type = 'const char*',
   no_cast = true,
   mapper = function (constant)
      if constant.msg and #constant.msg > 0 then
         return '"' .. constant.msg .. '"'
      else
         return '"An unknown error occurred"'
      end
   end,
   default = '"An unknown error occurred"'
}

include('common/enum_std_begin', result_code)
if file_ext == '.hpp' then
   write_template('common/templates/enum_scalar_mapping_decl', errc_map)
   write_template('common/templates/enum_scalar_mapping_decl', msg_map)
else
   write_template('common/templates/enum_scalar_mapping', errc_map)
   write_template('common/templates/enum_scalar_mapping', msg_map)
end
include('common/enum_std_end', result_code)
