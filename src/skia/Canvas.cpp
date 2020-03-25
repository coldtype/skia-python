#include <pybind11/pybind11.h>
#include <skia.h>

namespace py = pybind11;

void initCanvas(py::module &m) {
py::enum_<SkClipOp>(m, "ClipOp");

py::class_<SkCanvas> canvas(m, "Canvas");
py::enum_<SkCanvas::SrcRectConstraint>(canvas, "SrcRectConstraint")
    .value("kStrict_SrcRectConstraint",
        SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .value("kFast_SrcRectConstraint",
        SkCanvas::SrcRectConstraint::kFast_SrcRectConstraint)
    .export_values();
py::enum_<SkCanvas::PointMode>(canvas, "PointMode")
    .value("kPoints_PointMode", SkCanvas::PointMode::kPoints_PointMode)
    .value("kLines_PointMode", SkCanvas::PointMode::kLines_PointMode)
    .value("kPolygon_PointMode", SkCanvas::PointMode::kPolygon_PointMode)
    .export_values();
py::enum_<SkCanvas::QuadAAFlags>(canvas, "QuadAAFlags")
    .value("kLeft_QuadAAFlag", SkCanvas::QuadAAFlags::kLeft_QuadAAFlag)
    .value("kTop_QuadAAFlag", SkCanvas::QuadAAFlags::kTop_QuadAAFlag)
    .value("kRight_QuadAAFlag", SkCanvas::QuadAAFlags::kRight_QuadAAFlag)
    .value("kBottom_QuadAAFlag", SkCanvas::QuadAAFlags::kBottom_QuadAAFlag)
    .value("kNone_QuadAAFlags", SkCanvas::QuadAAFlags::kNone_QuadAAFlags)
    .value("kAll_QuadAAFlags", SkCanvas::QuadAAFlags::kAll_QuadAAFlags)
    .export_values();
// py::class_<SkCanvas::SaveLayerRec> savelayerrec(canvas, "SaveLayerRec");
// savelayerrec
//     .def_readwrite("fBounds", &SkCanvas::Lattice::fBounds)
//     .def_readwrite("fPaint", &SkCanvas::Lattice::fPaint)
//     .def_readwrite("fBackdrop", &SkCanvas::Lattice::fBackdrop)
//     .def_readwrite("fClipMask", &SkCanvas::Lattice::fClipMask)
//     .def_readwrite("fClipMatrix", &SkCanvas::Lattice::fClipMatrix)
//     .def_readwrite("fSaveLayerFlags", &SkCanvas::Lattice::fSaveLayerFlags)
//     ;
py::class_<SkCanvas::Lattice> lattice(canvas, "Lattice");
lattice
    .def_readwrite("fXDivs", &SkCanvas::Lattice::fXDivs)
    .def_readwrite("fYDivs", &SkCanvas::Lattice::fYDivs)
    .def_readwrite("fRectTypes", &SkCanvas::Lattice::fRectTypes)
    .def_readwrite("fXCount", &SkCanvas::Lattice::fXCount)
    .def_readwrite("fYCount", &SkCanvas::Lattice::fYCount)
    .def_readwrite("fBounds", &SkCanvas::Lattice::fBounds)
    .def_readwrite("fColors", &SkCanvas::Lattice::fColors)
    ;
py::enum_<SkCanvas::Lattice::RectType>(lattice, "RectType")
    .value("kDefault", SkCanvas::Lattice::RectType::kDefault)
    .value("kTransparent", SkCanvas::Lattice::RectType::kTransparent)
    .value("kFixedColor", SkCanvas::Lattice::RectType::kFixedColor)
    .export_values();

canvas.def(py::init<>(),
        "Creates an empty SkCanvas with no backing device or pixels, with a "
        "width and height of zero.")
    .def(py::init<int, int, const SkSurfaceProps*>(),
        "Creates SkCanvas of the specified dimensions without a SkSurface.")
    .def(py::init<const SkBitmap&>(),
        "Constructs a canvas that draws into bitmap.")
    .def(py::init<const SkBitmap&, const SkSurfaceProps&>(),
        "Constructs a canvas that draws into bitmap.")
    .def("imageInfo", &SkCanvas::imageInfo, "Returns SkImageInfo for SkCanvas.")
    .def("getProps", &SkCanvas::getProps,
        "Copies SkSurfaceProps, if SkCanvas is associated with raster surface "
        "or GPU surface, and returns true.")
    .def("flush", &SkCanvas::flush,
        "Triggers the immediate execution of all pending draw operations.")
    .def("getBaseLayerSize", &SkCanvas::getBaseLayerSize,
        "Gets the size of the base or root layer in global canvas coordinates.")
    .def("makeSurface", &SkCanvas::makeSurface,
        "Creates SkSurface matching info and props, and associates it with "
        "SkCanvas.")
    .def("getGrContext", &SkCanvas::getGrContext,
        "Returns GPU context of the GPU surface associated with SkCanvas.")
    .def("getSurface", &SkCanvas::getSurface,
        "Sometimes a canvas is owned by a surface.")
    .def("accessTopLayerPixels", &SkCanvas::accessTopLayerPixels,
        "Returns the pixel base address, SkImageInfo, rowBytes, and origin if "
        "the pixels can be read directly.",
        py::return_value_policy::reference)
    // .def("accessTopRasterHandle", &SkCanvas::accessTopRasterHandle,
    //     "Returns custom context that tracks the SkMatrix and clip.")
    .def("peekPixels", &SkCanvas::peekPixels,
        "Returns true if SkCanvas has direct access to its pixels.")
    .def("readPixels",
        (bool (SkCanvas::*)(const SkImageInfo&, void*, size_t, int, int))
        &SkCanvas::readPixels,
        "Copies SkRect of pixels from SkCanvas into dstPixels.")
    .def("readPixels",
        (bool (SkCanvas::*)(const SkPixmap&, int, int)) &SkCanvas::readPixels,
        "Copies SkRect of pixels from SkCanvas into pixmap.")
    .def("readPixels",
        (bool (SkCanvas::*)(const SkBitmap&, int, int)) &SkCanvas::readPixels,
        "Copies SkRect of pixels from SkCanvas into bitmap.")
    .def("writePixels",
        (bool (SkCanvas::*)(const SkImageInfo&, const void*, size_t, int, int))
        &SkCanvas::writePixels,
        "Copies SkRect from pixels to SkCanvas.")
    .def("writePixels",
        (bool (SkCanvas::*)(const SkBitmap&, int, int)) &SkCanvas::writePixels,
        "Copies SkRect from pixels to SkCanvas.")
    .def("save", &SkCanvas::save, "Saves SkMatrix and clip.")
    .def("saveLayer",
        (int (SkCanvas::*)(const SkRect*, const SkPaint*)) &SkCanvas::saveLayer,
        "Saves SkMatrix and clip, and allocates a SkBitmap for subsequent "
        "drawing.")
    .def("saveLayer",
        (int (SkCanvas::*)(const SkRect&, const SkPaint*)) &SkCanvas::saveLayer,
        "Saves SkMatrix and clip, and allocates a SkBitmap for subsequent "
        "drawing.")
    .def("saveLayerAlpha", &SkCanvas::saveLayerAlpha,
        "Saves SkMatrix and clip, and allocates SkBitmap for subsequent "
        "drawing.")
    // .def("saveLayer",
    //     (int (SkCanvas::*)(const SaveLayerRec&)) &SkCanvas::saveLayer,
    //     "Saves SkMatrix and clip, and allocates a SkBitmap for subsequent "
    //     "drawing.")
    .def("experimental_saveCamera",
        (int (SkCanvas::*)(const SkM44&, const SkM44&))
        &SkCanvas::experimental_saveCamera)
    .def("experimental_saveCamera",
        (int (SkCanvas::*)(const SkScalar[16], const SkScalar[16]))
        &SkCanvas::experimental_saveCamera)
    .def("restore", &SkCanvas::restore,
        "Removes changes to SkMatrix and clip since SkCanvas state was last "
        "saved.")
    .def("getSaveCount", &SkCanvas::getSaveCount,
        "Returns the number of saved states, each containing: SkMatrix and "
        "clip.")
    .def("restoreToCount", &SkCanvas::restoreToCount,
        "Restores state to SkMatrix and clip values when save(), saveLayer(), "
        "saveLayerPreserveLCDTextRequests(), or saveLayerAlpha() returned "
        "saveCount.")
    .def("translate", &SkCanvas::translate,
        "Translates SkMatrix by dx along the x-axis and dy along the y-axis.")
    .def("scale", &SkCanvas::scale,
        "Scales SkMatrix by sx on the x-axis and sy on the y-axis.")
    .def("rotate", (void (SkCanvas::*)(SkScalar)) &SkCanvas::rotate,
        "Rotates SkMatrix by degrees.")
    .def("rotate",
        (void (SkCanvas::*)(SkScalar, SkScalar, SkScalar)) &SkCanvas::rotate,
        "Rotates SkMatrix by degrees about a point at (px, py).")
    .def("skew", &SkCanvas::skew,
        "Skews SkMatrix by sx on the x-axis and sy on the y-axis.")
    .def("concat", &SkCanvas::concat,
        "Replaces SkMatrix with matrix premultiplied with existing SkMatrix.")
    .def("concat44", (void (SkCanvas::*)(const SkM44&)) &SkCanvas::concat44)
    .def("concat44", (void (SkCanvas::*)(const SkScalar[])) &SkCanvas::concat44)
    .def("setMatrix", &SkCanvas::setMatrix, "Replaces SkMatrix with matrix.")
    .def("resetMatrix", &SkCanvas::resetMatrix,
        "Sets SkMatrix to the identity matrix.")
    .def("clipRect",
        (void (SkCanvas::*)(const SkRect&, SkClipOp, bool)) &SkCanvas::clipRect,
        "Replaces clip with the intersection or difference of clip and rect, "
        "with an aliased or anti-aliased clip edge.")
    .def("clipRect",
        (void (SkCanvas::*)(const SkRect&, SkClipOp)) &SkCanvas::clipRect,
        "Replaces clip with the intersection or difference of clip and rect.")
    .def("clipRect",
        (void (SkCanvas::*)(const SkRect&, bool)) &SkCanvas::clipRect,
        "Replaces clip with the intersection of clip and rect.",
        py::arg("rect"), py::arg("doAntiAlias") = false)
    .def("androidFramework_setDeviceClipRestriction",
        &SkCanvas::androidFramework_setDeviceClipRestriction,
        "Sets the maximum clip rectangle, which can be set by clipRect(), "
        "clipRRect() and clipPath() and intersect the current clip with the "
        "specified rect.")
    .def("clipRRect",
        (void (SkCanvas::*)(const SkRRect&, SkClipOp, bool))
        &SkCanvas::clipRRect,
        "Replaces clip with the intersection or difference of clip and rrect, "
        "with an aliased or anti-aliased clip edge.")
    .def("clipRRect",
        (void (SkCanvas::*)(const SkRRect&, SkClipOp)) &SkCanvas::clipRRect,
        "Replaces clip with the intersection or difference of clip and rrect.")
    .def("clipRect",
        (void (SkCanvas::*)(const SkRRect&, bool)) &SkCanvas::clipRRect,
        "Replaces clip with the intersection of clip and rrect, with an "
        "aliased or anti-aliased clip edge.",
        py::arg("rrect"), py::arg("doAntiAlias") = false)
    .def("clipPath",
        (void (SkCanvas::*)(const SkPath&, SkClipOp, bool)) &SkCanvas::clipPath,
        "Replaces clip with the intersection or difference of clip and path, "
        "with an aliased or anti-aliased clip edge.")
    .def("clipPath",
        (void (SkCanvas::*)(const SkPath&, SkClipOp)) &SkCanvas::clipPath,
        "Replaces clip with the intersection or difference of clip and path.")
    .def("clipPath",
        (void (SkCanvas::*)(const SkPath&, bool)) &SkCanvas::clipPath,
        "Replaces clip with the intersection of clip and path.",
        py::arg("path"), py::arg("doAntiAlias") = false)
    // .def("clipShader", &SkCanvas::clipShader)
    .def("clipRegion", &SkCanvas::clipRegion,
        "Replaces clip with the intersection or difference of clip and "
        "SkRegion deviceRgn.")
    .def("quickReject",
        (bool (SkCanvas::*)(const SkRect&) const) &SkCanvas::quickReject,
        "Returns true if SkRect rect, transformed by SkMatrix, can be quickly "
        "determined to be outside of clip.")
    .def("quickReject",
        (bool (SkCanvas::*)(const SkPath&) const) &SkCanvas::quickReject,
        "Returns true if path, transformed by SkMatrix, can be quickly "
        "determined to be outside of clip.")
    .def("getLocalClipBounds",
        (SkRect (SkCanvas::*)() const) &SkCanvas::getLocalClipBounds,
        "Returns bounds of clip, transformed by inverse of SkMatrix.")
    .def("getLocalClipBounds",
        (bool (SkCanvas::*)(SkRect*) const) &SkCanvas::getLocalClipBounds,
        "Returns bounds of clip, transformed by inverse of SkMatrix.")
    .def("getDeviceClipBounds",
        (SkIRect (SkCanvas::*)() const) &SkCanvas::getDeviceClipBounds,
        "Returns SkIRect bounds of clip, unaffected by SkMatrix.")
    .def("getDeviceClipBounds",
        (bool (SkCanvas::*)(SkIRect*) const) &SkCanvas::getDeviceClipBounds,
        "Returns SkIRect bounds of clip, unaffected by SkMatrix.")
    .def("drawColor", &SkCanvas::drawColor, "Fills clip with color color.")
    .def("clear", &SkCanvas::clear,
        "Fills clip with color using SkBlendMode::kSrc")
    .def("discard", &SkCanvas::discard, "Makes SkCanvas contents undefined.")
    .def("drawPaint", &SkCanvas::drawPaint, "Fills clip with SkPaint paint.")
    .def("drawPoints", &SkCanvas::drawPoints,
        "Draws pts using clip, SkMatrix and SkPaint paint.")
    .def("drawPoint",
        (void (SkCanvas::*)(SkScalar, SkScalar, const SkPaint&))
        &SkCanvas::drawPoint,
        "Draws point at (x, y) using clip, SkMatrix and SkPaint paint.")
    .def("drawPoint",
        (void (SkCanvas::*)(SkPoint, const SkPaint&))
        &SkCanvas::drawPoint,
        "Draws point p using clip, SkMatrix and SkPaint paint.")
    .def("drawLine",
        (void (SkCanvas::*)(SkScalar, SkScalar, SkScalar, SkScalar,
            const SkPaint&)) &SkCanvas::drawLine,
        "Draws line segment from (x0, y0) to (x1, y1) using clip, SkMatrix, "
        "and SkPaint paint.")
    .def("drawLine",
        (void (SkCanvas::*)(SkPoint, SkPoint, const SkPaint&))
        &SkCanvas::drawLine,
        "Draws line segment from p0 to p1 using clip, SkMatrix, and SkPaint "
        "paint.")
    .def("drawRect", &SkCanvas::drawRect,
        "Draws SkRect rect using clip, SkMatrix, and SkPaint paint.")
    .def("drawIRect", &SkCanvas::drawIRect,
        "Draws SkIRect rect using clip, SkMatrix, and SkPaint paint.")
    .def("drawRegion", &SkCanvas::drawRegion,
        "Draws SkRegion region using clip, SkMatrix, and SkPaint paint.")
    .def("drawOval", &SkCanvas::drawOval,
        "Draws oval oval using clip, SkMatrix, and SkPaint.")
    .def("drawRRect", &SkCanvas::drawRRect,
        "Draws SkRRect rrect using clip, SkMatrix, and SkPaint paint.")
    .def("drawDRRect", &SkCanvas::drawDRRect,
        "Draws SkRRect outer and inner using clip, SkMatrix, and SkPaint "
        "paint.")
    .def("drawCircle",
        (void (SkCanvas::*)(SkScalar, SkScalar, SkScalar, const SkPaint&))
        &SkCanvas::drawCircle,
        "Draws circle at (cx, cy) with radius using clip, SkMatrix, and "
        "SkPaint paint.")
    .def("drawCircle",
        (void (SkCanvas::*)(SkPoint, SkScalar, const SkPaint&))
        &SkCanvas::drawCircle,
        "Draws circle at center with radius using clip, SkMatrix, and SkPaint "
        "paint.")
    .def("drawArc", &SkCanvas::drawArc,
        "Draws arc using clip, SkMatrix, and SkPaint paint.")
    .def("drawRoundRect", &SkCanvas::drawRoundRect,
        "Draws SkRRect bounded by SkRect rect, with corner radii (rx, ry) "
        "using clip, SkMatrix, and SkPaint paint.")
    .def("drawPath", &SkCanvas::drawPath,
        "Draws SkPath path using clip, SkMatrix, and SkPaint paint.")
    .def("drawImage",
        (void (SkCanvas::*)(const SkImage*, SkScalar, SkScalar,
            const SkPaint*)) &SkCanvas::drawImage,
        "Draws SkImage image, with its top-left corner at (left, top), using "
        "clip, SkMatrix, and optional SkPaint paint")
    .def("drawImage",
        (void (SkCanvas::*)(const sk_sp<SkImage>&, SkScalar, SkScalar,
            const SkPaint*)) &SkCanvas::drawImage,
        "Draws SkImage image, with its top-left corner at (left, top), using "
        "clip, SkMatrix, and optional SkPaint paint")
    .def("drawImageRect",
        (void (SkCanvas::*)(const SkImage*, const SkRect&, const SkRect&,
            const SkPaint*, SkCanvas::SrcRectConstraint))
        &SkCanvas::drawImageRect,
        "Draws SkRect src of SkImage image, scaled and translated to fill "
        "SkRect dst.",
        py::arg("image"), py::arg("src"), py::arg("dst"), py::arg("paint"),
        py::arg("constraint") =
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .def("drawImageRect",
        (void (SkCanvas::*)(const SkImage*, const SkIRect&, const SkRect&,
            const SkPaint*, SkCanvas::SrcRectConstraint))
        &SkCanvas::drawImageRect,
        "Draws SkIRect isrc of SkImage image, scaled and translated to fill "
        "SkRect dst.",
        py::arg("image"), py::arg("isrc"), py::arg("dst"), py::arg("paint"),
        py::arg("constraint") =
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .def("drawImageRect",
        (void (SkCanvas::*)(const SkImage*, const SkRect&, const SkPaint*))
        &SkCanvas::drawImageRect,
        "Draws SkImage image, scaled and translated to fill SkRect dst, using "
        "clip, SkMatrix, and optional SkPaint paint.")
    .def("drawImageRect",
        (void (SkCanvas::*)(const sk_sp<SkImage>&, const SkRect&, const SkRect&,
            const SkPaint*, SkCanvas::SrcRectConstraint))
        &SkCanvas::drawImageRect,
        "Draws SkRect src of SkImage image, scaled and translated to fill "
        "SkRect dst.",
        py::arg("image"), py::arg("src"), py::arg("dst"), py::arg("paint"),
        py::arg("constraint") =
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .def("drawImageRect",
        (void (SkCanvas::*)(const sk_sp<SkImage>&, const SkIRect&,
            const SkRect&, const SkPaint*, SkCanvas::SrcRectConstraint))
        &SkCanvas::drawImageRect,
        "Draws SkIRect isrc of SkImage image, scaled and translated to fill "
        "SkRect dst.",
        py::arg("image"), py::arg("isrc"), py::arg("dst"), py::arg("paint"),
        py::arg("constraint") =
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .def("drawImageRect",
        (void (SkCanvas::*)(const sk_sp<SkImage>&, const SkRect&,
            const SkPaint*)) &SkCanvas::drawImageRect,
        "Draws SkImage image, scaled and translated to fill SkRect dst, using "
        "clip, SkMatrix, and optional SkPaint paint.")
    .def("drawImageNine",
        (void (SkCanvas::*)(const SkImage*, const SkIRect&, const SkRect&,
            const SkPaint*)) &SkCanvas::drawImageNine,
        "Draws SkImage image stretched proportionally to fit into SkRect dst.")
    .def("drawImageNine",
        (void (SkCanvas::*)(const sk_sp<SkImage>&, const SkIRect&,
            const SkRect&, const SkPaint*)) &SkCanvas::drawImageNine,
        "Draws SkImage image stretched proportionally to fit into SkRect dst.")
    .def("drawBitmap", &SkCanvas::drawBitmap,
        "Draws SkBitmap bitmap, with its top-left corner at (left, top), using "
        "clip, SkMatrix, and optional SkPaint paint.")
    .def("drawBitmapRect",
        (void (SkCanvas::*)(const SkBitmap&, const SkRect&, const SkRect&,
            const SkPaint*, SkCanvas::SrcRectConstraint))
        &SkCanvas::drawBitmapRect,
        "Draws SkRect src of SkBitmap bitmap, scaled and translated to fill "
        "SkRect dst.",
        py::arg("bitmap"), py::arg("src"), py::arg("dst"), py::arg("paint"),
        py::arg("constraint") =
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .def("drawBitmapRect",
        (void (SkCanvas::*)(const SkBitmap&, const SkIRect&, const SkRect&,
            const SkPaint*, SkCanvas::SrcRectConstraint))
        &SkCanvas::drawBitmapRect,
        "Draws SkIRect isrc of SkBitmap bitmap, scaled and translated to fill "
        "SkRect dst.",
        py::arg("bitmap"), py::arg("isrc"), py::arg("dst"), py::arg("paint"),
        py::arg("constraint") =
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .def("drawBitmapRect",
        (void (SkCanvas::*)(const SkBitmap&, const SkRect&, const SkPaint*,
            SkCanvas::SrcRectConstraint)) &SkCanvas::drawBitmapRect,
        "Draws SkBitmap bitmap, scaled and translated to fill SkRect dst.",
        py::arg("bitmap"), py::arg("dst"), py::arg("paint"),
        py::arg("constraint") =
            SkCanvas::SrcRectConstraint::kStrict_SrcRectConstraint)
    .def("drawImageLattice", &SkCanvas::drawImageLattice,
        "Draws SkImage image stretched proportionally to fit into SkRect dst.")
    .def("experimental_DrawEdgeAAQuad",
        (void (SkCanvas::*)(const SkRect&, const SkPoint[4],
            SkCanvas::QuadAAFlags, const SkColor4f&, SkBlendMode))
        &SkCanvas::experimental_DrawEdgeAAQuad,
        "This is an experimental API for the SkiaRenderer Chromium project, "
        "and its API will surely evolve if it is not removed outright.")
    .def("experimental_DrawEdgeAAQuad",
        (void (SkCanvas::*)(const SkRect&, const SkPoint[4],
            SkCanvas::QuadAAFlags, SkColor, SkBlendMode))
        &SkCanvas::experimental_DrawEdgeAAQuad)
    .def("drawSimpleText", &SkCanvas::drawSimpleText,
        "Draws text, with origin at (x, y), using clip, SkMatrix, SkFont font, "
        "and SkPaint paint.")
    .def("drawString",
        (void (SkCanvas::*)(const char[], SkScalar, SkScalar, const SkFont&,
            const SkPaint&)) &SkCanvas::drawString,
        "Draws null terminated string, with origin at (x, y), using clip, "
        "SkMatrix, SkFont font, and SkPaint paint.")
    // .def("drawString",
    //     (void (SkCanvas::*)(const SkString&, SkScalar, SkScalar, const SkFont&,
    //         const SkPaint&)) &SkCanvas::drawString,
    //     "DDraws SkString, with origin at (x, y), using clip, SkMatrix, SkFont "
    //     "font, and SkPaint paint.")
    .def("drawTextBlob",
        (void (SkCanvas::*)(const SkTextBlob*, SkScalar, SkScalar,
            const SkPaint&)) &SkCanvas::drawTextBlob,
        "Draws SkTextBlob blob at (x, y), using clip, SkMatrix, and SkPaint "
        "paint.")
    .def("drawTextBlob",
        (void (SkCanvas::*)(const sk_sp<SkTextBlob>&, SkScalar, SkScalar,
            const SkPaint&)) &SkCanvas::drawTextBlob,
        "Draws SkTextBlob blob at (x, y), using clip, SkMatrix, and SkPaint "
        "paint.")
    .def("drawPicture",
        (void (SkCanvas::*)(const SkPicture*)) &SkCanvas::drawPicture,
        "Draws SkPicture picture, using clip and SkMatrix.")
    .def("drawPicture",
        (void (SkCanvas::*)(const sk_sp<SkPicture>&)) &SkCanvas::drawPicture,
        "Draws SkPicture picture, using clip and SkMatrix.")
    .def("drawPicture",
        (void (SkCanvas::*)(const SkPicture*, const SkMatrix*, const SkPaint*))
        &SkCanvas::drawPicture,
        "Draws SkPicture picture, using clip and SkMatrix; transforming "
        "picture with SkMatrix matrix, if provided; and use SkPaint paint "
        "alpha, SkColorFilter, SkImageFilter, and SkBlendMode, if provided.")
    .def("drawPicture",
        (void (SkCanvas::*)(const sk_sp<SkPicture>&, const SkMatrix*,
            const SkPaint*)) &SkCanvas::drawPicture,
        "Draws SkPicture picture, using clip and SkMatrix; transforming "
        "picture with SkMatrix matrix, if provided; and use SkPaint paint "
        "alpha, SkColorFilter, SkImageFilter, and SkBlendMode, if provided.")
    .def("drawVertices",
        (void (SkCanvas::*)(const SkVertices*, SkBlendMode, const SkPaint&))
        &SkCanvas::drawVertices,
        "Draws SkVertices vertices, a triangle mesh, using clip and SkMatrix.")
    .def("drawVertices",
        (void (SkCanvas::*)(const SkVertices*, const SkPaint&))
        &SkCanvas::drawVertices,
        "Variant of 3-parameter drawVertices, using the default of Modulate "
        "for the blend parameter.")
    .def("drawVertices",
        (void (SkCanvas::*)(const sk_sp<SkVertices>&, SkBlendMode,
            const SkPaint&)) &SkCanvas::drawVertices,
        "Draws SkVertices vertices, a triangle mesh, using clip and SkMatrix.")
    .def("drawVertices",
        (void (SkCanvas::*)(const sk_sp<SkVertices>&, const SkPaint&))
        &SkCanvas::drawVertices,
        "Variant of 3-parameter drawVertices, using the default of Modulate "
        "for the blend parameter.")
    .def("drawPatch",
        (void (SkCanvas::*)(const SkPoint[12], const SkColor[4],
            const SkPoint[4], SkBlendMode, const SkPaint&))
        &SkCanvas::drawPatch,
        "Draws a Coons patch: the interpolation of four cubics with shared "
        "corners, associating a color, and optionally a texture SkPoint, with "
        "each corner.")
    .def("drawPatch",
        (void (SkCanvas::*)(const SkPoint[12], const SkColor[4],
            const SkPoint[4], const SkPaint&))
        &SkCanvas::drawPatch,
        "Draws SkPath cubic Coons patch: the interpolation of four cubics with "
        "shared corners, associating a color, and optionally a texture "
        "SkPoint, with each corner.")
    .def("drawAtlas",
        (void (SkCanvas::*)(const SkImage*, const SkRSXform[], const SkRect[],
            const SkColor[], int, SkBlendMode, const SkRect*, const SkPaint*))
        &SkCanvas::drawAtlas,
        "Draws a set of sprites from atlas, using clip, SkMatrix, and optional "
        "SkPaint paint.")
    .def("drawAtlas",
        (void (SkCanvas::*)(const sk_sp<SkImage>&, const SkRSXform[],
            const SkRect[], const SkColor[], int, SkBlendMode, const SkRect*,
            const SkPaint*)) &SkCanvas::drawAtlas,
        "Draws a set of sprites from atlas, using clip, SkMatrix, and optional "
        "SkPaint paint.")
    .def("drawAtlas",
        (void (SkCanvas::*)(const SkImage*, const SkRSXform[], const SkRect[],
            int, const SkRect*, const SkPaint*))
        &SkCanvas::drawAtlas,
        "Draws a set of sprites from atlas, using clip, SkMatrix, and optional "
        "SkPaint paint.")
    .def("drawAtlas",
        (void (SkCanvas::*)(const sk_sp<SkImage>&, const SkRSXform[],
            const SkRect[], int, const SkRect*, const SkPaint*))
        &SkCanvas::drawAtlas,
        "Draws a set of sprites from atlas, using clip, SkMatrix, and optional "
        "SkPaint paint.")
    /*
    .def("drawDrawable",
        (void (SkCanvas::*)(SkDrawable*, const SkMatrix*))
        &SkCanvas::drawDrawable,
        "Draws SkDrawable drawable using clip and SkMatrix, concatenated with "
        "optional matrix.")
    .def("drawDrawable",
        (void (SkCanvas::*)(SkDrawable*, SkScalar, SkScalar))
        &SkCanvas::drawDrawable,
        "Draws SkDrawable drawable using clip and SkMatrix, offset by (x, y).")
    */
    .def("drawAnnotation",
        (void (SkCanvas::*)(const SkRect&, const char[], SkData*))
        &SkCanvas::drawAnnotation,
        "Associates SkRect on SkCanvas with an annotation; a key-value pair, "
        "where the key is a null-terminated UTF-8 string, and optional value "
        "is stored as SkData.")
    .def("drawAnnotation",
        (void (SkCanvas::*)(const SkRect&, const char[], const sk_sp<SkData>&))
        &SkCanvas::drawAnnotation,
        "Associates SkRect on SkCanvas when an annotation; a key-value pair, "
        "where the key is a null-terminated UTF-8 string, and optional value "
        "is stored as SkData.")
    .def("isClipEmpty", &SkCanvas::isClipEmpty,
        "Returns true if clip is empty; that is, nothing will draw.")
    .def("isClipRect", &SkCanvas::isClipRect,
        "Returns true if clip is SkRect and not empty.")
    .def("getTotalMatrix", &SkCanvas::getTotalMatrix, "Returns SkMatrix.")
    .def("getLocalToDevice",
        (SkM44 (SkCanvas::*)() const) &SkCanvas::getLocalToDevice)
    .def("getLocalToDevice",
        (void (SkCanvas::*)(SkScalar[16]) const) &SkCanvas::getLocalToDevice)
    .def("experimental_getLocalToWorld",
        (SkM44 (SkCanvas::*)() const) &SkCanvas::experimental_getLocalToWorld)
    .def("experimental_getLocalToCamera",
        (SkM44 (SkCanvas::*)() const) &SkCanvas::experimental_getLocalToCamera)
    .def("experimental_getLocalToCamera",
        (void (SkCanvas::*)(SkScalar[16]) const)
        &SkCanvas::experimental_getLocalToCamera)
    .def("experimental_getLocalToWorld",
        (void (SkCanvas::*)(SkScalar[16]) const)
        &SkCanvas::experimental_getLocalToWorld)
    // Static methods.
    .def_static("MakeRasterDirect", &SkCanvas::MakeRasterDirect,
        "Allocates raster SkCanvas that will draw directly into pixels.")
    .def_static("MakeRasterDirectN32", &SkCanvas::MakeRasterDirectN32,
        "Allocates raster SkCanvas specified by inline image specification.")
    ;
}