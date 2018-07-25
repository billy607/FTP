#ifndef LABELINGFIELDNAMES
#define LABELINGFIELDNAMES


#define ATTRIBUTE_VALIDATE_NAME "valid"
#define ATTRIBUTE_VALIDATE_INVALDATE "false"
#define ATTRIBUTE_VALIDATE_VALDATE "true"

#define SHAPE_SOURCE "source" //框的来源
#define SHAPE_SOURCE_CALC "calc"//算法框

#define FILTER_OPS "ops"
#define FILTER_OPS_DELETED "deleted"

#define SHAPE_NAME_RECTS "rects"
#define SHAPE_NAME_POLYGONS "polygons"
#define SHAPE_NAME_LINES "lines"
#define SHAPE_NAME_MULPOINTS "multiPoints"


//工具的说明，如果任务发布时RectTool的公共工具有一个显示说明的titile
#define TOOL_DESC_FIELD "desc-field"





//配置文件字段
//////////////////////////////////////////////////////////////////////////
/*
<step target = "rects" field="trafficSign" toolId ="66" toolName="RectTool">
*/
#define STEP_TARGET_NAME "target"
#define STEP_TOOLID_NAME "toolId"
#define STEP_TOOLNAME_NAME "toolName"
#define STEP_FEILD_NAME "field"

//添加可选信息
/*
<option field="field" value="ops"/>
*/
#define OPTION_NAME_FIELD "field"
#define OPTION_NAME_VALUE "value"

//配置文件中绑定快捷键
/*
 *
 * <key-values>
 *      <entry key="lclick" value="deleted"/>
 *      <entry key="rclick" value="verified"/>
 * </key-values>
 *
 */
#define KEY_VALUE_NAME "key-values"
#define KEY_VALUE_NAME_KEY "key"
#define KEY_VALUE_NAME_VALUE "value"


//配置文件中绑定值与Label
/*
<value-label>
    <param value = "deleted" label="不是人脸" />
    <param value = "verified" label="是人脸"/>
</value-label>
*/
#define VALUE_LABEL_NAME "value-label"
#define VALUE_LABEL_NAME_VALUE "value"
#define VALUE_LABEL_NAME_LABEL "label"



#define RECT_SOURCE_NAME "source"
#define RECT_SOURCE_ALGORITHM "calc"
#define RECT_SOURCE_LABELING "labeling"


#define RECT_OPS_NAME "ops"
#define RECT_OPS_DELETED "deleted"
#define RECT_OPS_INVALID "invalid"
#define RECT_OPS_VERIFIED "verified"

// 是否有金样例及金样例位置

#define HAS_GOLDEN_STANDARD "hasGoldStandard"
#define STATISTICS_DLL_NAME "statisticsDll"
#define CHECK_FUNCTION_NAME "compareLabelWithGoldStandard"
#define CHECK_RESULT_FILE "result.dll"


#endif // LABELINGFIELDNAMES

